#include "input.h"

void Input::update()
{
    for (auto it = mouse_hold_info.begin();it!= mouse_hold_info.end();)
    {
        if (it->second.released)
        {
            it = mouse_hold_info.erase(it);
        }
        else it++;
    }    
}

void Input::update(const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved)
    {
        for (auto& button_hold_info : mouse_hold_info)
        {
            button_hold_info.second.hold_info.current_pos = { event.mouseMove.x, event.mouseMove.y };
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        const sf::Vector2i mouse_pos = { event.mouseButton.x, event.mouseButton.y };
        mouse_hold_info[event.mouseButton.button] = Mouse_hold_info_impl{ {mouse_pos, mouse_pos}, false };
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        mouse_hold_info[event.mouseButton.button].released = true;
    }
}

std::optional<Input::Mouse_hold_info> Input::getMouseHoldVec(sf::Mouse::Button button)
{
    auto it = mouse_hold_info.find(button);
    if (it != mouse_hold_info.end()) return { it->second.hold_info };

    return {};
}