#include "port/achievements.h"

#include "ui/ui.hpp"

#include <algorithm>
#include <filesystem>
#include <port/io.hpp>
#include <port/main.h>
#include <port/settings.h>

namespace partyboard {

using json = nlohmann::json;

static void* s_cucco_play_search(void* i_actor, void*) {
    // if (!fopAcM_IsActor(i_actor) || fopAcM_GetName((fopAc_ac_c*)i_actor) != fpcNm_NI_e) {
    //     return nullptr;
    // }
    // auto* ni = static_cast<ni_class*>(i_actor);
    // return ni->mAction == ACTION_PLAY_e ? i_actor : nullptr;
}

// static void checkGoatHerding(Achievement& a, int32_t threshMs) {
//     if (dMeter2Info_getMaxCount() != 20 || dMeter2Info_getNowCount() != 20) {
//         return;
//     }
//     const int32_t elapsed = dMeter2Info_getTimeMs();
//     if (elapsed > 0 && elapsed <= threshMs) {
//         a.progress = 1;
//     }
// }

static constexpr auto ACHIEVEMENTS_FILENAME = "achievements.json";

std::vector<AchievementSystem::Entry> AchievementSystem::makeEntries() {
    return {
    };
}

AchievementSystem::AchievementSystem() : m_entries(makeEntries()) {}

AchievementSystem& AchievementSystem::get() {
    static AchievementSystem instance;
    return instance;
}

std::vector<Achievement> AchievementSystem::getAchievements() const {
    std::vector<Achievement> result;
    result.reserve(m_entries.size());
    for (const auto& e : m_entries) {
        result.push_back(e.achievement);
    }
    return result;
}

void AchievementSystem::load() {
    m_loaded = true;
    const auto filePath = PartyBoard_ConfigPath / ACHIEVEMENTS_FILENAME;
    if (!std::filesystem::exists(filePath)) {
        return;
    }
    try {
        auto data = io::FileStream::ReadAllBytes(filePath);
        auto j = json::parse(data);
        if (!j.is_object()) {
            return;
        }
        for (auto& e : m_entries) {
            if (!j.contains(e.achievement.key)) {
                continue;
            }
            const auto& entry = j[e.achievement.key];
            if (entry.contains("progress")) {
                e.achievement.progress = entry["progress"].get<int32_t>();
            }
            if (entry.contains("unlocked")) {
                e.achievement.unlocked = entry["unlocked"].get<bool>();
            }
            if (entry.contains("extra")) {
                e.extra = entry["extra"];
            }
        }
    } catch (const std::exception&) {}
}

void AchievementSystem::save() {
    json j = json::object();
    for (const auto& e : m_entries) {
        json entry = {
            {"progress", e.achievement.progress},
            {"unlocked", e.achievement.unlocked},
        };
        if (!e.extra.is_null()) {
            entry["extra"] = e.extra;
        }
        j[e.achievement.key] = std::move(entry);
    }
    try {
        io::FileStream::WriteAllText(
            PartyBoard_ConfigPath / ACHIEVEMENTS_FILENAME,
            j.dump(2)
        );
    } catch (const std::exception&) {}
}

void AchievementSystem::clearAll() {
    m_entries = makeEntries();
    save();
}

void AchievementSystem::signal(const char* key) {
    m_signals.insert(key);
}

bool AchievementSystem::hasSignal(const char* key) const {
    return m_signals.count(key) > 0;
}

void AchievementSystem::clearOne(const char* key) {
    for (auto& e : m_entries) {
        if (std::string(e.achievement.key) == key) {
            e.achievement.progress = 0;
            e.achievement.unlocked = false;
            e.extra = {};
            break;
        }
    }
    save();
}

void AchievementSystem::processEntry(Entry& e) {
    if (e.achievement.unlocked) {
        return;
    }
    const int32_t prevProgress = e.achievement.progress;
    e.check(e.achievement, e.extra);

    const bool progressChanged = e.achievement.progress != prevProgress;
    const bool nowUnlocked = e.achievement.isCounter ?
        e.achievement.progress >= e.achievement.goal :
        e.achievement.progress > 0;

    if (nowUnlocked) {
        e.achievement.progress = e.achievement.isCounter ? e.achievement.goal : 1;
        e.achievement.unlocked = true;
        if (getSettings().game.enableAchievementToasts) {
            ui::push_toast({
                .type = "achievement",
                .title = "Achievement Unlocked!",
                .content = e.achievement.name,
                .duration = std::chrono::seconds(5),
            });
        }
        m_dirty = true;
    } else if (progressChanged) {
        m_dirty = true;
    }
}

void AchievementSystem::tick() {
    if (!m_loaded) {
        load();
    }
    if (!PartyBoard_IsGameLaunched) {
        return;
    }
    for (auto& e : m_entries) {
        processEntry(e);
    }
    m_signals.clear();
    if (m_dirty) {
        save();
        m_dirty = false;
    }
}

} // namespace partyboard
