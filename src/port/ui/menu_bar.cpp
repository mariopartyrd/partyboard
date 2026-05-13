// Credits: TwilitRealm

#include "menu_bar.hpp"

#include <RmlUi/Core.h>

#include "achievements.hpp"
#include "aurora/rmlui.hpp"
#include "port/settings.h"
#include "imgui.h"
#include "modal.hpp"
#include "settings.hpp"
#include "ui.hpp"
#include "window.hpp"

#include <chrono>
#include <cmath>

namespace partyboard::ui {
namespace {

    const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/tabbing.rcss" />
    <link type="text/rcss" href="res/rml/popup.rcss" />
</head>
<body>
    <popup id="popup" />
</body>
</rml>
)RML";

}

MenuBar::MenuBar()
    : Document(kDocumentSource)
    , mRoot(mDocument->GetElementById("popup"))
{
    mTabBar = std::make_unique<TabBar>(mRoot,
        TabBar::Props {
            .onClose =
                [this] {
                    // mDoAud_seStartMenu(kSoundMenuClose); // TODO
                    hide(false);
                },
            .autoSelect = false,
        });
    mTabBar->add_tab("Settings", [this] { push(std::make_unique<SettingsWindow>()); });
    // mTabBar->add_tab("Warp", [] {
    //     // TODO
    // });

    mTabBar->add_tab("Achievements", [this] { push(std::make_unique<AchievementsWindow>()); });
    // TODO PC
    // mTabBar->add_tab("Reset", [this] {
    //     mTabBar->set_active_tab(-1);
    //     const auto dismiss = [](Modal &modal) { modal.pop(); };
    //     push(std::make_unique<Modal>(Modal::Props{
    //         .title = "Reset Game",
    //         .bodyRml = "Unsaved progress will be lost.<br/>"
    //                    "<span class=\"tip\">Tip: You can also reset by holding Start+X+B</span>",
    //         .actions =
    //             {
    //                 ModalAction{
    //                     .label = "Cancel",
    //                     .onPressed =
    //                         [this, dismiss](Modal& modal) {
    //                             // mDoAud_seStartMenu(kSoundWindowClose); // TODO PC
    //                             dismiss(modal);
    //                         },
    //                 },
    //                 // ModalAction{
    //                 //     .label = "Reset",
    //                 //     .onPressed =
    //                 //         [this, dismiss](Modal& modal) {
    //                 //             // mDoAud_seStartMenu(kSoundClick); // TODO PC
    //                 //             // if (fpcM_SearchByName(fpcNm_LOGO_SCENE_e)) {
    //                 //             //     dismiss(modal);
    //                 //             //     return;
    //                 //             // }
    //                 //             dismiss(modal);
    //                 //             hide(false);
    //                 //         },
    //                 // },
    //             },
    //         .onDismiss = dismiss,
    //         .icon = "question-mark",
    //     }));
    // });
    mTabBar->add_tab("Quit", [this] {
        mTabBar->set_active_tab(-1);
        const auto dismiss = [](Modal &modal) { modal.pop(); };
        push(std::make_unique<Modal>(Modal::Props{
            .title = "Quit Party Board",
            .bodyRml = "Unsaved progress will be lost.",
            .actions =
                {
                    ModalAction{
                        .label = "Cancel",
                        .onPressed =
                            [dismiss](Modal& modal) {
                                // mDoAud_seStartMenu(kSoundWindowClose); // TODO PC
                                dismiss(modal);
                            },
                    },
                    ModalAction{
                        .label = "Quit",
                        .onPressed =
                            [dismiss](Modal& modal) {
                                // mDoAud_seStartMenu(kSoundClick); // TODO PC
                                dismiss(modal);
                                // IsRunning = false; // TODO PC
                            },
                    },
                },
            .onDismiss = dismiss,
            .icon = "question-mark",
        }));
    });

    // Hide document after transition completion
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event &event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") && Document::visible()) {
            Document::hide(mPendingClose);
        }
    });
}

void MenuBar::show()
{
    Document::show();
    mRoot->SetAttribute("open", "");
    mTabBar->set_active_tab(-1);
    if (!mTabBar->focus_tab(mFocusedTabIndex)) {
        mTabBar->focus();
    }
}

void MenuBar::hide(bool close)
{
    mFocusedTabIndex = mTabBar->focused_tab_index();
    mRoot->RemoveAttribute("open");
    if (close) {
        mPendingClose = true;
    }
}

void MenuBar::update()
{
    update_safe_area();
    Document::update();
}

void MenuBar::update_safe_area() noexcept
{
    if (mDocument == nullptr || mTabBar == nullptr) {
        return;
    }

    // Avoid ImGui menu bar if shown
    if (const auto *viewport = ImGui::GetMainViewport(); viewport != nullptr && mTopMargin != viewport->WorkPos.y) {
        mTopMargin = viewport->WorkPos.y;
        mRoot->SetProperty(Rml::PropertyId::MarginTop, Rml::Property(mTopMargin, Rml::Unit::DP));
    }

    Rml::Context *context = mDocument->GetContext();
    Insets safeInsets = safe_area_insets(context);
    safeInsets = {
        0.0f,
        std::round(safeInsets.right),
        0.0f,
        std::round(safeInsets.left),
    };
    if (safeInsets == mTabBarPadding) {
        return;
    }

    mTabBarPadding = safeInsets;
    auto *tabBar = mTabBar->root();
    tabBar->SetProperty(Rml::PropertyId::PaddingRight, Rml::Property(safeInsets.right, Rml::Unit::PX));
    tabBar->SetProperty(Rml::PropertyId::PaddingLeft, Rml::Property(safeInsets.left, Rml::Unit::PX));
    if (auto *close = tabBar->QuerySelector("close")) {
        close->SetProperty(
            Rml::PropertyId::Right, Rml::Property(safeInsets.right + 8.0f * context->GetDensityIndependentPixelRatio(), Rml::Unit::PX));
    }
}

bool MenuBar::visible() const
{
    return mRoot->HasAttribute("open");
}

bool MenuBar::handle_nav_command(Rml::Event &event, NavCommand cmd)
{
    if (!getSettings().backend.wasPresetChosen) {
        return true;
    }
    if (cmd == NavCommand::Cancel && visible()) {
        // mDoAud_seStartMenu(kSoundMenuClose); // TODO PC
        hide(false);
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

bool MenuBar::focus()
{
    return mTabBar->focus();
}

} // namespace partyboard::ui
