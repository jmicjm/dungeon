#include "player_inventory.h"
#include "../primitives/apperance_common.h"

void gui::Player_inventory::redraw()
{
    bg.draw();
    text.draw();
    btn.draw();
}

void gui::Player_inventory::activateEvent()
{
    text.activate();
    btn.activate();
}

void gui::Player_inventory::deactivateEvent()
{
    text.deactivate();
    btn.deactivate();
}

gui::Player_inventory::Player_inventory()
{
    bg.setParent(this);
    text.setParent(this);
    btn.setParent(this);

    bg.setSizeInfo({ {0,0}, {1,1} });
    text.setSizeInfo({ {0,-32}, {1,1} });
    btn.setSizeInfo({ {128,32} });
    btn.setPositionInfo({ {0,0}, {0,0}, {1,1} });

    text.setString("this text serves the purpose of distracting you from the fact that this screen is not yet implemented");

    bg.setAppearance({ rect({41, 32, 22, 224}) });

    btn.setPressedHoveredText("TEST");
    btn.setReleasedText("test");
    btn.setReleasedHoveredText("TEST");

    auto btn_f = [=]
    {
        class Dialog : public Gui_component, public Component_stack_helper
        {
            gui::Frame bg;
            gui::Text text;
            gui::Button close_btn;

            void redraw() override 
            {
                bg.draw();
                text.draw(); 
                close_btn.draw();
            }
            void activateEvent() override
            {
                text.activate();
                close_btn.activate();
            }
            void deactivateEvent() override
            {
                text.deactivate();
                close_btn.deactivate();
            }
            void update() override
            {
                text.update();
                close_btn.update();
            }
        public:
            Dialog(const Frame_appearance& fa)
            {
                bg.setParent(this);
                bg.setSizeInfo({ .percentage = {1,1} });
                bg.setAppearance(fa);
                text.setParent(this);
                text.setString("same purpose, different text");
                text.setSizeInfo({ {0,-24}, {1,1} });
                close_btn.setParent(this);
                close_btn.setSizeInfo({ { 96, 24 } });
                close_btn.setPositionInfo({ .relative_to = {1,1} });
                close_btn.setReleasedText("close");
                close_btn.setReleasedHoveredText("close");
                close_btn.setPressedHoveredText("close");
                close_btn.setPressFunction([&] { removeFromComponentStack(); });
                close_btn.setFontSize(16);
            }
        };
        auto dialog = std::make_unique<Dialog>(Frame_appearance{ rect({0,0,0,255}) });
        dialog->setSizeInfo({ .percentage = {0.3,0.2} });
        dialog->setPositionInfo({ .relative_to = {0.5,0.5} });
        Component_stack::Component_config cfg;
        cfg.outside_click_action = Component_stack::Component_config::OUTSIDE_CLICK_ACTION::NOTHING;
        insertIntoComponentStack(std::move(dialog), cfg);
    };
    btn.setPressFunction(btn_f);
}

void gui::Player_inventory::update()
{
    text.update();
    btn.update();
}
