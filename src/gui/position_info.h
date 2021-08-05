#pragma once
#include "SFML/System/Vector2.hpp"

namespace gui
{
    struct Position_info
    {
        sf::Vector2i offset;
        sf::Vector2f percentage_offset;
        sf::Vector2f relative_to;
    };
}