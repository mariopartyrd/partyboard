// Credits: TwilitRealm

#pragma once

#include "port/achievements.h"
#include "window.hpp"

#include <vector>

namespace partyboard::ui {

class AchievementsWindow : public Window {
public:
    AchievementsWindow();
    void update() override;

private:
    void updateTotal();
    std::vector<Achievement> mSnapshot;
    Rml::Element *mTotalEl = nullptr;
};

} // namespace partyboard::ui
