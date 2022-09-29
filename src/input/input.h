#include <optional>
#include <map>
#include <set>

#include "SFML/Window/Event.hpp"


class Input
{
    struct Mouse_hold_info
    {
        sf::Vector2i source_pos;
        sf::Vector2i current_pos;
    };
    struct Mouse_hold_info_impl
    {
        Mouse_hold_info hold_info;
        bool released;
    };
    inline static std::map<sf::Mouse::Button, Mouse_hold_info_impl> mouse_hold_info;
    inline static std::map<sf::Mouse::Button, bool> mouse_pressed_buttons;
    inline static std::map<sf::Mouse::Button, bool> mouse_released_buttons;

public:

    static void update();
    static void update(const sf::Event& event);

    static std::optional<Mouse_hold_info> getMouseHoldVec(sf::Mouse::Button button);
    static bool isPressed(sf::Mouse::Button button);
    static bool isPressedConsume(sf::Mouse::Button button);
    static bool isReleased(sf::Mouse::Button button);
    static bool isReleasedConsume(sf::Mouse::Button button);
    static bool isHold(sf::Mouse::Button button);
};