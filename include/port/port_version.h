#pragma once

#ifdef __cplusplus

#include <cstdlib>
#include <dvd.h>
#include <initializer_list>
#include <types.h>
#include "version.h"

/**
 * Functionality for switching game behavior based on the loaded game version (e.g. PAL/JPN, GC/Wii)
 */
namespace partyboard::version {
enum class GameVersion : u8 {
    UsaRev0 = VERSION_NO_ENG0,
    UsaRev1 = VERSION_NO_ENG1,
    PalRev0 = VERSION_NO_PAL0,
    PalRev1 = VERSION_NO_PAL1,
    PalRev2 = VERSION_NO_PAL2,
    Jpn = VERSION_NO_JP,
};

bool isRegionPal();
bool isRegionJpn();
bool isRegionUsa();

GameVersion getGameVersion();

const DVDDiskID& getDiskID();

void init();

template<typename T>
struct VersionOption {
    GameVersion mVersion;
    T mValue;

    constexpr VersionOption(GameVersion version, T value) : mVersion(version), mValue(value) {}
};

template<typename T>
const T& versionSelect(const std::initializer_list<VersionOption<T>> options) {
    const auto version = getGameVersion();
    for (const auto& opt : options) {
        if (opt.mVersion == version) {
            return opt.mValue;
        }
    }

    // Unable to find value.
    abort();
}

template<typename T>
const T& versionSelect(const std::initializer_list<VersionOption<T>> options, const T& defaultValue) {
    const auto version = getGameVersion();
    for (const auto& opt : options) {
        if (opt.mVersion == version) {
            return opt.mValue;
        }
    }

    return defaultValue;
}
}  // namespace partyboard::version

#else

u8 partyboard_version_get_version(void);

BOOL partyboard_version_is_pal(void);
BOOL partyboard_version_is_usa(void);
BOOL partyboard_version_is_ntsc(void);

const char* partyboard_version_get_game_name(void);
const char* partyboard_version_get_company(void);

#endif