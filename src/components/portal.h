#pragma once
#include "../level/level.fwd.h"

#include "SFML/System/Vector2.hpp"

#include <memory>


struct Portal
{
    std::weak_ptr<Level> destination_level;
    sf::Vector2i destination_coords;
};