// Credits: TwilitRealm

#ifndef PARTY_BOARD_CONFIG_H
#define PARTY_BOARD_CONFIG_H

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <card.h>

#ifdef __cplusplus
#include "port/config_var.hpp"

#include <os/OSRtc.h>

namespace partyboard {

using namespace config;

enum class GameLanguage : u8 {
    English = OS_LANGUAGE_ENGLISH,
    German = OS_LANGUAGE_GERMAN,
    French = OS_LANGUAGE_FRENCH,
    Spanish = OS_LANGUAGE_SPANISH,
    Italian = OS_LANGUAGE_ITALIAN,
};

enum class DiscVerificationState : u8 {
    Unknown = 0,
    Success,
    HashMismatch,
};

namespace config {
template <>
struct ConfigEnumRange<GameLanguage> {
    static constexpr auto min = GameLanguage::English;
    static constexpr auto max = GameLanguage::Italian;
};

template <>
struct ConfigEnumRange<DiscVerificationState> {
    static constexpr auto min = DiscVerificationState::Unknown;
    static constexpr auto max = DiscVerificationState::HashMismatch;
};

}

// Persistent user settings

struct UserSettings {
    // Program settings

    struct {
        // Video
        ConfigVar<bool> enableFullscreen;
        ConfigVar<bool> enableVsync;
        ConfigVar<bool> lockAspectRatio;
        ConfigVar<bool> enableFpsOverlay;
        ConfigVar<int> fpsOverlayCorner;
    } video;

    struct {
        // Audio
        ConfigVar<int> masterVolume;
        ConfigVar<int> mainMusicVolume;
        ConfigVar<int> subMusicVolume;
        ConfigVar<int> soundEffectsVolume;
        ConfigVar<int> fanfareVolume;
        ConfigVar<bool> enableReverb;
        ConfigVar<bool> enableHrtf;
        ConfigVar<bool> menuSounds;
    } audio;

    // Game settings

    struct {
        ConfigVar<GameLanguage> language;

        // QoL
        ConfigVar<bool> enableQuickTransform;

        // Preferences
        ConfigVar<bool> pauseOnFocusLost;
        ConfigVar<bool> enableAchievementToasts;
        ConfigVar<bool> enableControllerToasts;

        // Graphics
        ConfigVar<int> internalResolutionScale;
        ConfigVar<int> shadowResolutionMultiplier;

        // Audio

        // Input
        ConfigVar<bool> allowBackgroundInput;

        // Cheats
        ConfigVar<bool> infiniteHearts;
        ConfigVar<bool> unlockAllMinigames;

        // Technical

        // Controls
        ConfigVar<bool> enableTurboKeybind;

        // Tools
        ConfigVar<bool> speedrunMode;
        ConfigVar<bool> recordingMode;
    } game;

    struct {
        ConfigVar<std::string> isoPath;
        ConfigVar<DiscVerificationState> isoVerification;
        ConfigVar<std::string> graphicsBackend;
        ConfigVar<bool> skipPreLaunchUI;
        ConfigVar<bool> showPipelineCompilation;
        ConfigVar<bool> wasPresetChosen;
        ConfigVar<bool> enableCrashReporting;
        ConfigVar<bool> checkForUpdates;
        ConfigVar<int> cardFileType;
        ConfigVar<bool> enableAdvancedSettings;
    } backend;
};

UserSettings& getSettings();

void registerSettings();

}
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

CARDFileType partyboard_settings_card_file_type(void);
bool partyboard_settings_enableTurboKeybind(void);
bool partyboard_settings_unlock_all_minigames(void);

#ifdef __cplusplus
}
#endif

#endif // PARTY_BOARD_CONFIG_H
