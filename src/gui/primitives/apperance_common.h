#pragma once
#include "SFML/Graphics/RectangleShape.hpp"

namespace gui
{
    inline sf::RectangleShape rect(sf::Color c)
    {
        sf::RectangleShape rs;
        rs.setFillColor(c);
        return rs;
    }
}