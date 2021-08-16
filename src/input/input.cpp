#include "input.h"

void Input::update()
{
    if (mouse_hold_info && mouse_hold_info->released) mouse_hold_info.reset();
}

void Input::update(const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
       if(mouse_hold_info) mouse_hold_info->current_pos = { event.mouseMove.x, event.mouseMove.y };
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        const sf::Vector2i mouse_pos = { event.mouseButton.x, event.mouseButton.y };
        mouse_hold_info = Mouse_hold_info{ mouse_pos, mouse_pos, false };
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        if (mouse_hold_info) mouse_hold_info->released = true;
    }
}

std::optional<Input::Mouse_hold_info> Input::getMouseHoldVec()
{
    return mouse_hold_info;
}