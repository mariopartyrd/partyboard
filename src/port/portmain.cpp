// Credits: TwilitRealm

#include "imgui/ImGuiEngine.hpp"
#include "iso_validate.hpp"
#include "ui/ui.hpp"

#include "partyboard_version.h"
#include "ui/menu_bar.hpp"
#include "ui/overlay.hpp"
#include "ui/prelaunch.hpp"
#include "ui/preset.hpp"

#include <SDL3/SDL_filesystem.h>
#include <aurora/aurora.h>
#include <aurora/event.h>
#include <dolphin/gx/GXAurora.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include <game/disp.h>
#include <port/config.hpp>
#include <port/dolassets.h>
#include <port/main.h>
#include <port/settings.h>

#include <aurora/dvd.h>
#include <aurora/lib/logging.hpp>
#include <stdio.h>
#include <stdlib.h>

extern "C" int game_main();

using namespace std::string_literals;
using namespace std::string_view_literals;

static bool mainCalled = false;

AuroraInfo auroraInfo;
std::filesystem::path PartyBoard_ConfigPath;

aurora::Module PartyBoardMainLog("partyboard::main");

bool launchUILoop() {
    while (PartyBoard_IsRunning && !PartyBoard_IsGameLaunched) {
        const AuroraEvent* event = aurora_update();
        while (event != nullptr && event->type != AURORA_NONE) {
            switch (event->type) {
                case AURORA_SDL_EVENT:
                    partyboard::ui::handle_event(event->sdl);
                // partyboard::g_imguiConsole.HandleSDLEvent(event->sdl);
                break;
                case AURORA_DISPLAY_SCALE_CHANGED:
                    // TODO PC
                    // partyboard::ImGuiEngine_Initialize(event->windowSize.scale);
                break;
                case AURORA_EXIT:
                    return false;
            }

            event++;
        }

        if (!aurora_begin_frame()) {
            PartyBoardMainLog.debug("aurora_begin_frame returned false, skipping draw this frame");
            continue;
        }

        partyboard::ui::update();

        // partyboard::g_imguiConsole.PreDraw();
        // partyboard::g_imguiConsole.PostDraw();

        aurora_end_frame();
    }

    return PartyBoard_IsRunning;
}

void aurora_log_callback(AuroraLogLevel level, const char* module, const char *message, unsigned int len)
{
    const char *levelStr = "??";
    FILE *out = stdout;
    switch (level) {
        case LOG_DEBUG:
            levelStr = "DEBUG";
            break;
        case LOG_INFO:
            levelStr = "INFO";
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            out = stderr;
            break;
        case LOG_FATAL:
            levelStr = "FATAL";
            out = stderr;
            break;
    }
    fprintf(out, "[%s | %s] %s\n", levelStr, module, message);
    if (level == LOG_FATAL) {
        fflush(out);
        abort();
    }
}

static bool try_parse_backend(std::string_view backend, AuroraBackend& outBackend) {
    if (backend == "auto") {
        outBackend = BACKEND_AUTO;
        return true;
    }
    if (backend == "d3d11") {
        outBackend = BACKEND_D3D11;
        return true;
    }
    if (backend == "d3d12") {
        outBackend = BACKEND_D3D12;
        return true;
    }
    if (backend == "metal") {
        outBackend = BACKEND_METAL;
        return true;
    }
    if (backend == "vulkan") {
        outBackend = BACKEND_VULKAN;
        return true;
    }
    if (backend == "opengl") {
        outBackend = BACKEND_OPENGL;
        return true;
    }
    if (backend == "opengles") {
        outBackend = BACKEND_OPENGLES;
        return true;
    }
    if (backend == "webgpu") {
        outBackend = BACKEND_WEBGPU;
        return true;
    }
    if (backend == "null") {
        outBackend = BACKEND_NULL;
        return true;
    }

    return false;
}

static std::string_view backend_name(AuroraBackend backend) {
    switch (backend) {
        default:
            return "Auto"sv;
        case BACKEND_D3D12:
            return "D3D12"sv;
        case BACKEND_D3D11:
            return "D3D11"sv;
        case BACKEND_METAL:
            return "Metal"sv;
        case BACKEND_VULKAN:
            return "Vulkan"sv;
        case BACKEND_OPENGL:
            return "OpenGL"sv;
        case BACKEND_OPENGLES:
            return "OpenGL ES"sv;
        case BACKEND_WEBGPU:
            return "WebGPU"sv;
        case BACKEND_NULL:
            return "Null"sv;
    }
}

static bool IsBackendAvailable(AuroraBackend backend) {
    if (backend == BACKEND_AUTO) {
        return true;
    }

    size_t availableBackendCount = 0;
    const AuroraBackend* availableBackends = aurora_get_available_backends(&availableBackendCount);
    for (size_t i = 0; i < availableBackendCount; ++i) {
        if (availableBackends[i] == backend) {
            return true;
        }
    }

    return false;
}

static AuroraBackend ResolveDesiredBackend() {
    AuroraBackend desiredBackend = BACKEND_AUTO;

    if (!try_parse_backend(
                   static_cast<const std::string&>(partyboard::getSettings().backend.graphicsBackend),
                   desiredBackend))
    {
        PartyBoardMainLog.warn("Unknown configured backend '{}', falling back to Auto",
                     static_cast<const std::string&>(partyboard::getSettings().backend.graphicsBackend));
        desiredBackend = BACKEND_AUTO;
    }

    if (!IsBackendAvailable(desiredBackend)) {
        PartyBoardMainLog.warn("Requested backend '{}' is unavailable, falling back to Auto",
                     backend_name(desiredBackend));
        desiredBackend = BACKEND_AUTO;
    }

    return desiredBackend;
}


static void migrate_directory(const std::filesystem::path& from, const std::filesystem::path& to) {
    std::error_code ec;
    std::filesystem::create_directories(to, ec);
    if (ec) {
        return;
    }

    for (std::filesystem::recursive_directory_iterator it(
             from, std::filesystem::directory_options::skip_permission_denied, ec);
        it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
    {
        if (ec) {
            return;
        }

        const auto relativePath = std::filesystem::relative(it->path(), from, ec);
        if (ec) {
            return;
        }

        const auto targetPath = to / relativePath;
        if (it->is_directory(ec)) {
            std::filesystem::create_directories(targetPath, ec);
            if (ec) {
                return;
            }
        } else if (it->is_regular_file(ec) && !std::filesystem::exists(targetPath, ec)) {
            std::filesystem::create_directories(targetPath.parent_path(), ec);
            if (ec) {
                return;
            }
            std::filesystem::copy_file(
                it->path(), targetPath, std::filesystem::copy_options::skip_existing, ec);
            if (ec) {
                return;
            }
        }
    }
}

static std::filesystem::path calculate_config_path() {
#ifdef __APPLE__
#if TARGET_OS_IOS && !TARGET_OS_TV
    const char* documentsPath = SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS);
    if (!documentsPath) {
        PartyBoardMainLog.error("Unable to get iOS Documents path: {}", SDL_GetError());
    }

    std::filesystem::path configPath = reinterpret_cast<const char8_t*>(documentsPath);

    char* oldPrefPath = SDL_GetPrefPath("MarioPartyRD", "Party Board");
    if (oldPrefPath) {
        const std::filesystem::path oldConfigPath = reinterpret_cast<const char8_t*>(oldPrefPath);
        SDL_free(oldPrefPath);

        std::error_code ec;
        if (oldConfigPath != configPath && std::filesystem::exists(oldConfigPath, ec)) {
            migrate_directory(oldConfigPath, configPath);
        }
    }

    return configPath;
#endif
#endif

    const auto result = SDL_GetPrefPath("MarioPartyRD", "Party Board");
    if (!result) {
        PartyBoardMainLog.error("Unable to get PrefPath: {}", SDL_GetError());
    }

    return reinterpret_cast<const char8_t*>(result);
}

static void EnsureInitialPipelineCache(const std::filesystem::path& configDir) {
    if (configDir.empty()) {
        return;
    }

    const std::filesystem::path pipelineCachePath = configDir / "pipeline_cache.db";
    if (std::filesystem::exists(pipelineCachePath)) {
        return;
    }

    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        PartyBoardMainLog.error("Unable to resolve base path while seeding pipeline cache: {}", SDL_GetError());
        return;
    }

    const std::filesystem::path initialPipelineCachePath =
        std::filesystem::path(basePath) / "initial_pipeline_cache.db";
    if (!std::filesystem::exists(initialPipelineCachePath)) {
        PartyBoardMainLog.error("No bundled initial pipeline cache found at '{}'", initialPipelineCachePath.string());
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(configDir, ec);
    if (ec) {
        PartyBoardMainLog.error("Failed to create config directory '{}' for pipeline cache: {}",
                     configDir.string(), ec.message());
        return;
    }

    std::filesystem::copy_file(initialPipelineCachePath, pipelineCachePath, std::filesystem::copy_options::none, ec);
    if (ec) {
        PartyBoardMainLog.error("Failed to seed pipeline cache from '{}' to '{}': {}",
                     initialPipelineCachePath.string(), pipelineCachePath.string(), ec.message());
        return;
    }

    PartyBoardMainLog.info("Seeded pipeline cache from '{}'", initialPipelineCachePath.string());
}

static constexpr PADDefaultMapping defaultPadMapping = {
    .buttons = {
        {SDL_GAMEPAD_BUTTON_SOUTH, PAD_BUTTON_A},
        {SDL_GAMEPAD_BUTTON_EAST, PAD_BUTTON_B},
        {SDL_GAMEPAD_BUTTON_WEST, PAD_BUTTON_X},
        {SDL_GAMEPAD_BUTTON_NORTH, PAD_BUTTON_Y},
        {SDL_GAMEPAD_BUTTON_START, PAD_BUTTON_START},
        {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, PAD_TRIGGER_Z},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_L},
        {PAD_NATIVE_BUTTON_INVALID, PAD_TRIGGER_R},
        {SDL_GAMEPAD_BUTTON_DPAD_UP, PAD_BUTTON_UP},
        {SDL_GAMEPAD_BUTTON_DPAD_DOWN, PAD_BUTTON_DOWN},
        {SDL_GAMEPAD_BUTTON_DPAD_LEFT, PAD_BUTTON_LEFT},
        {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, PAD_BUTTON_RIGHT},
    },
    .axes = {
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_POS},
        {{SDL_GAMEPAD_AXIS_LEFTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_X_NEG},
        // SDL's gamepad y-axis is inverted from GC's
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_POS},
        {{SDL_GAMEPAD_AXIS_LEFTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_LEFT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTX, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_X_NEG},
        // see above
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_NEGATIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_POS},
        {{SDL_GAMEPAD_AXIS_RIGHTY, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_RIGHT_Y_NEG},
        {{SDL_GAMEPAD_AXIS_LEFT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_L},
        {{SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, AXIS_SIGN_POSITIVE}, SDL_GAMEPAD_BUTTON_INVALID, PAD_AXIS_TRIGGER_R},
    },
};

extern "C" int port_main(int argc, char* argv[]) {
    // On iOS, when connected to an external monitor, SDLUIKitSceneDelegate scene:willConnectToSession:
    // can call our main function again. Explicitly guard against this reinitialization.
    if (mainCalled) {
        return 0;
    }
    mainCalled = true;

    partyboard::registerSettings();
    partyboard::config::FinishRegistration();

    PartyBoard_ConfigPath = calculate_config_path();

    partyboard::config::LoadFromUserPreferences();
    EnsureInitialPipelineCache(PartyBoard_ConfigPath);
    // TODO: How to handle this?
    //PADSetDefaultMapping(&defaultPadMapping, PAD_TYPE_STANDARD);

    {
        const auto configPathString = PartyBoard_ConfigPath.u8string();
        AuroraConfig config{};
        config.appName = "Party Board";
        config.userPath = reinterpret_cast<const char*>(configPathString.c_str());
        config.vsync = partyboard::getSettings().video.enableVsync;
        config.startFullscreen = partyboard::getSettings().video.enableFullscreen;
        config.windowPosX = -1;
        config.windowPosY = -1;
        config.windowWidth = HU_FB_WIDTH * 2;
        config.windowHeight = HU_FB_HEIGHT * 2;
        config.desiredBackend = ResolveDesiredBackend();
        config.logCallback = &aurora_log_callback;
        config.mem1Size = 64 * 1024 * 1024;
        config.mem2Size = 24 * 1024 * 1024;
        config.allowJoystickBackgroundEvents = partyboard::getSettings().game.allowBackgroundInput;
        config.pauseOnFocusLost = partyboard::getSettings().game.pauseOnFocusLost;
        // config.imGuiInitCallback = &aurora_imgui_init_callback;
        config.allowTextureReplacements = true;
        config.allowTextureDumps = false;
        auroraInfo = aurora_initialize(argc, argv, &config);
    }

#ifdef PARTY_BOARD_DISCORD
    partyboard::discord::initialize();
#endif

    char windowTitle[100];
    snprintf(windowTitle, sizeof(windowTitle), "PartyBoard %s", PARTY_BOARD_WC_DESCRIBE);
    VISetWindowTitle(windowTitle);

    if (partyboard::getSettings().video.lockAspectRatio) {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
    } else {
        AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
    }
    VISetFrameBufferScale(partyboard::getSettings().game.internalResolutionScale.getValue());

    // TODO PC
    // partyboard::audio::SetMasterVolume(partyboard::getSettings().audio.masterVolume / 100.0f);
    // partyboard::audio::SetEnableReverb(partyboard::getSettings().audio.enableReverb);
    // partyboard::audio::EnableHrtf = partyboard::getSettings().audio.enableHrtf;

    // Run ImGui UI loop if Aurora couldn't initialize a backend
    if (auroraInfo.backend == BACKEND_NULL) {
        launchUILoop();
        fflush(stdout);
        fflush(stderr);
#ifdef PARTY_BOARD_DISCORD
        partyboard::discord::shutdown();
#endif
        partyboard::ui::shutdown();
        aurora_shutdown();
        return 0;
    }

    partyboard::ui::initialize();
    partyboard::ui::push_document(std::make_unique<partyboard::ui::Overlay>(), true, true);
    partyboard::ui::push_document(std::make_unique<partyboard::ui::MenuBar>(), false);

    // Invalidate a bad saved isoPath so that Party Board can't get blocked from starting up.
    // This is only a metadata check; full hash verification is handled by the prelaunch UI.
    bool forcePreLaunchUI = false;
    bool saveConfigBeforePrelaunch = false;

    const std::string p = partyboard::getSettings().backend.isoPath;
    partyboard::iso::DiscInfo discInfo{};
    if (!p.empty() &&
        partyboard::iso::inspect(p.c_str(), discInfo) != partyboard::iso::ValidationError::Success)
    {
        PartyBoardMainLog.info("Saved DVD image path failed validation, clearing configured path: {}", p);
        partyboard::getSettings().backend.isoPath.setValue("");
        partyboard::getSettings().backend.isoVerification.setValue(partyboard::DiscVerificationState::Unknown);
        forcePreLaunchUI = true;
        saveConfigBeforePrelaunch = true;
    }

    partyboard::iso::log_verification_state(
        partyboard::getSettings().backend.isoPath.getValue(),
        partyboard::getSettings().backend.isoVerification.getValue());

    if (partyboard::getSettings().backend.isoPath.getValue().empty()) {
        forcePreLaunchUI = true;
    }
    if (forcePreLaunchUI && partyboard::getSettings().backend.skipPreLaunchUI.getValue()) {
        PartyBoardMainLog.info("Prelaunch UI was disabled with no usable DVD image, enabling prelaunch UI");
        partyboard::getSettings().backend.skipPreLaunchUI.setValue(false);
        saveConfigBeforePrelaunch = true;
    }
    if (saveConfigBeforePrelaunch) {
        partyboard::config::Save();
    }

    if (!partyboard::getSettings().backend.skipPreLaunchUI) {
        partyboard::ui::push_document(std::make_unique<partyboard::ui::Prelaunch>(), true);

        // pre game launch ui main loop
        if (!launchUILoop()) {
            fflush(stdout);
            fflush(stderr);
#ifdef PARTY_BOARD_DISCORD
            partyboard::discord::shutdown();
#endif
            partyboard::ui::shutdown();
            aurora_shutdown();
            return 0;
        }
    }

     std::string dvd_path = partyboard::getSettings().backend.isoPath;

    if (dvd_path.empty()) {
        PartyBoardMainLog.error("No DVD image specified, unable to boot!");
    }
    if (!PartyBoard_IsGameLaunched &&
        partyboard::iso::inspect(dvd_path.c_str(), discInfo) != partyboard::iso::ValidationError::Success)
    {
        PartyBoardMainLog.error("DVD image failed validation: {}", dvd_path);
    }
    PartyBoardMainLog.info("Loading DVD image: {}", dvd_path);
    if (!aurora_dvd_open(dvd_path.c_str())) {
        PartyBoardMainLog.error("Failed to open DVD image: {}", dvd_path);
    }

    PartyBoard_IsGameLaunched = true;

    if (!partyboard::getSettings().backend.wasPresetChosen) {
        partyboard::ui::push_document(std::make_unique<partyboard::ui::PresetWindow>());
    }

    // partyboard::version::init();
    // LanguageInit();

    // OSInit();

    // Reset Data
    // TODO PC
    // static mDoRstData sResetData = {0};
    // mDoRst::setResetData(&sResetData);
    // mDoRst::offReset();
    // mDoRst::setLogoScnFlag(0);

    InitializeDol();

    game_main();

    // partyboard::MoviePlayerShutdown();

    fflush(stdout);
    fflush(stderr);

    // Notifies all CVs and causes threads to exit
    OSResetSystem(OS_RESET_SHUTDOWN, 0, 0);

#ifdef PARTY_BOARD_DISCORD
    partyboard::discord::shutdown();
#endif
    partyboard::ui::shutdown();
    aurora_shutdown();

    return 0;
}
