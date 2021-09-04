#pragma once
#include "SFML/System/Vector2.hpp"

namespace gui
{
    struct Size_info
    {
        sf::Vector2f fixed;
        sf::Vector2f percentage;
    };
}