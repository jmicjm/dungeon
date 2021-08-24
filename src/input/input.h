#include <optional>
#include <map>

#include "SFML/Window/Event.hpp"


class Input
{
    struct Mouse_hold_info
    {
        sf::Vector2i source_pos;
        sf::Vector2i current_pos;
        bool released;
    };
    inline static std::map<sf::Mouse::Button, Mouse_hold_info> mouse_hold_info;

public:

    static void update();
    static void update(const sf::Event& event);

    static std::optional<Mouse_hold_info> getMouseHoldVec(sf::Mouse::Button button);
};