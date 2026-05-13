// Credits: TwilitRealm

#pragma once
#include "window.hpp"

namespace partyboard::ui {

class SettingsWindow : public Window {
public:
    SettingsWindow(bool prelaunch = false);

    void update() override;

protected:
    bool mPrelaunch;
};

} // namespace partyboard::ui
