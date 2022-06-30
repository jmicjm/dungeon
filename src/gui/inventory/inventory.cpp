#include "inventory.h"


void gui::Inventory::redraw()
{
    bg.draw();
    text.draw();
}

void gui::Inventory::activateEvent()
{
    text.activate();
}

void gui::Inventory::deactivateEvent()
{
    text.deactivate();
}

gui::Inventory::Inventory()
{
    bg.setParent(this);
    text.setParent(this);

    bg.setSizeInfo({ {0,0}, {1,1} });
    text.setSizeInfo({ {0,0}, {1,1} });


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
}

void gui::Inventory::update()
{
    text.update();
}
