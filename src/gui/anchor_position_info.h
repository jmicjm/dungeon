#pragma once
#include "SFML/System/Vector2.hpp"

namespace gui
{
    struct Anchor_position_info
    {
        enum SIDE
        {
            TOP, BOTTOM, LEFT, RIGHT
        };

        SIDE side = SIDE::TOP;
        sf::Vector2f offset = { 0,0 };
        float relative_to = 50;
    };
}