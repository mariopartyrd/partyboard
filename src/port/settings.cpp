#include "port/settings.h"
#include "port/config.hpp"

namespace partyboard {

UserSettings g_userSettings = {
    .video = {
        .enableFullscreen {"video.enableFullscreen", false},
        .enableVsync {"video.enableVsync", true},
        .lockAspectRatio {"video.lockAspectRatio", false},
        .enableFpsOverlay {"game.enableFpsOverlay", false},
        .fpsOverlayCorner {"game.fpsOverlayCorner", 0},
    },

    .audio = {
        .masterVolume {"audio.masterVolume", 80},
        .mainMusicVolume {"audio.mainMusicVolume", 100},
        .subMusicVolume {"audio.subMusicVolume", 100},
        .soundEffectsVolume {"audio.soundEffectsVolume", 100},
        .fanfareVolume {"audio.fanfareVolume", 100},
        .enableReverb {"audio.enableReverb", true},
        .enableHrtf {"audio.enableHrtf", false},
        .menuSounds {"audio.menuSounds", true},
    },

    .game = {
        .language { "game.language", GameLanguage::English },

        // Quality of Life
        .enableQuickTransform {"game.enableQuickTransform", false},

        // Preferences
        .pauseOnFocusLost {"game.pauseOnFocusLost", false},
        .enableAchievementToasts {"game.enableAchievementToasts", true},
        .enableControllerToasts {"game.enableControllerToasts", true},

        // Graphics
        .internalResolutionScale {"game.internalResolutionScale", 0},
        .shadowResolutionMultiplier {"game.shadowResolutionMultiplier", 1},

        // Audio

        // Input
        .allowBackgroundInput {"game.allowBackgroundInput", true},

        // Cheats
        .infiniteHearts {"game.infiniteHearts", false},
        .unlockAllMinigames {"game.unlockAllMinigames", false},

        // Technical

        // Controls
        .enableTurboKeybind {"game.enableTurboKeybind", false},

        // Tools
        .speedrunMode {"game.speedrunMode", false},
        .recordingMode {"game.recordingMode", false}
    },

    .backend = {
        .isoPath {"backend.isoPath", ""},
        .isoVerification {"backend.isoVerification", DiscVerificationState::Unknown},
        .graphicsBackend {"backend.graphicsBackend", "auto"},
        .skipPreLaunchUI {"backend.skipPreLaunchUI", false},
        .showPipelineCompilation {"backend.showPipelineCompilation", false},
        .wasPresetChosen {"backend.wasPresetChosen", false},
        .enableCrashReporting {"backend.enableCrashReporting", true},
        .checkForUpdates {"backend.checkForUpdates", true},
        .cardFileType {"backend.cardFileType", static_cast<int>(CARD_RAWIMAGE)},
        .enableAdvancedSettings {"backend.enableAdvancedSettings", false},
    }
};

UserSettings& getSettings() {
    return g_userSettings;
}

void registerSettings() {
    // Video
    Register(g_userSettings.video.enableFullscreen);
    Register(g_userSettings.video.enableVsync);
    Register(g_userSettings.video.lockAspectRatio);
    Register(g_userSettings.video.enableFpsOverlay);
    Register(g_userSettings.video.fpsOverlayCorner);

    // Audio
    Register(g_userSettings.audio.masterVolume);
    Register(g_userSettings.audio.mainMusicVolume);
    Register(g_userSettings.audio.subMusicVolume);
    Register(g_userSettings.audio.soundEffectsVolume);
    Register(g_userSettings.audio.fanfareVolume);
    Register(g_userSettings.audio.enableReverb);
    Register(g_userSettings.audio.enableHrtf);
    Register(g_userSettings.audio.menuSounds);

    // Game
    Register(g_userSettings.game.language);
    Register(g_userSettings.game.enableQuickTransform);
    Register(g_userSettings.game.pauseOnFocusLost);
    Register(g_userSettings.game.internalResolutionScale);
    Register(g_userSettings.game.shadowResolutionMultiplier);
    Register(g_userSettings.game.enableAchievementToasts);
    Register(g_userSettings.game.enableControllerToasts);
    Register(g_userSettings.game.enableTurboKeybind);
    Register(g_userSettings.game.speedrunMode);
    Register(g_userSettings.game.recordingMode);
    Register(g_userSettings.game.infiniteHearts);
    Register(g_userSettings.game.unlockAllMinigames);
    Register(g_userSettings.game.allowBackgroundInput);

    Register(g_userSettings.backend.isoPath);
    Register(g_userSettings.backend.isoVerification);
    Register(g_userSettings.backend.graphicsBackend);
    Register(g_userSettings.backend.skipPreLaunchUI);
    Register(g_userSettings.backend.showPipelineCompilation);
    Register(g_userSettings.backend.wasPresetChosen);
    Register(g_userSettings.backend.enableCrashReporting);
    Register(g_userSettings.backend.checkForUpdates);
    Register(g_userSettings.backend.cardFileType);
    Register(g_userSettings.backend.enableAdvancedSettings);
}

}

extern "C" {

CARDFileType partyboard_settings_card_file_type(void)
{
    return (CARDFileType)partyboard::getSettings().backend.cardFileType.getValue();
}

bool partyboard_settings_enableTurboKeybind(void)
{
    return partyboard::getSettings().game.enableTurboKeybind;
}

bool partyboard_settings_unlock_all_minigames(void)
{
    return partyboard::getSettings().game.unlockAllMinigames.getValue();
}

}
