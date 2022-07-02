#include "inventory.h"


void gui::Inventory::redraw()
{
    bg.draw();
    text.draw();
    btn.draw();
}

void gui::Inventory::activateEvent()
{
    text.activate();
    btn.activate();
}

void gui::Inventory::deactivateEvent()
{
    text.deactivate();
    btn.deactivate();
}

gui::Inventory::Inventory()
{
    bg.setParent(this);
    text.setParent(this);
    btn.setParent(this);

    bg.setSizeInfo({ {0,0}, {1,1} });
    text.setSizeInfo({ {0,-32}, {1,1} });
    btn.setSizeInfo({ {128,32} });
    btn.setPositionInfo({ {0,0}, {0,0}, {1,1} });


    auto rect = [](sf::Color c)
    {
        sf::RectangleShape rs;
        rs.setFillColor(c);
        return rs;
    };

    gui::Button_appearance ba = {
        .pressed = rect({ 255, 192, 192, 255 }),
        .released = rect({ 245, 203, 66, 255}),
        .pressed_hovered = rect({ 173, 144, 49, 255 }),
        .released_hovered = rect({ 209, 173, 54, 255 })
    };

    gui::Scroll_appearance sa = {
        .button_fixed_height = 16,
        .top_button = ba,
        .bottom_button = ba,      
        .handle = { rect({ 173, 144, 49, 255 }) }
    };

    gui::Text_appearance ta = {
        .scroll = sa,
        .scroll_width = 16,
    };

    text.setAppearance(ta);
    text.setFont("arial.ttf");
    text.setString("this text serves the purpose of distracting you from the fact that this screen is not yet implemented");

    bg.setAppearance({ rect({41, 32, 22, 224}) });

    btn.setAppearance(ba);
    btn.setFont("arial.ttf");
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
            Dialog(const Text_appearance& ta, const Button_appearance& ba, const Frame_appearance& fa)
            {
                bg.setParent(this);
                bg.setSizeInfo({ .percentage = {1,1} });
                bg.setAppearance(fa);
                text.setParent(this);
                text.setFont("arial.ttf");
                text.setString("same purpose, different text");
                text.setAppearance(ta);
                text.setSizeInfo({ {0,-24}, {1,1} });
                close_btn.setParent(this);
                close_btn.setSizeInfo({ { 96, 24 } });
                close_btn.setPositionInfo({ .relative_to = {1,1} });
                close_btn.setAppearance(ba);
                close_btn.setFont("arial.ttf");
                close_btn.setReleasedText("close");
                close_btn.setReleasedHoveredText("close");
                close_btn.setPressedHoveredText("close");
                close_btn.setPressFunction([&] { removeFromComponentStack(); });
                close_btn.setFontSize(16);
            }
        };
        auto dialog = std::make_unique<Dialog>(ta, ba, Frame_appearance{ rect({0,0,0,255}) });
        dialog->setSizeInfo({ .percentage = {0.3,0.2} });
        dialog->setPositionInfo({ .relative_to = {0.5,0.5} });
        Component_stack::Component_config cfg;
        cfg.outside_click_action = Component_stack::Component_config::OUTSIDE_CLICK_ACTION::NOTHING;
        insertIntoComponentStack(std::move(dialog), cfg);
    };
    btn.setPressFunction(btn_f);
}

void gui::Inventory::update()
{
    text.update();
    btn.update();
}
