#pragma once
#include "../level_gen/gen_params.h"
#include "SFML/System/Vector2.hpp"

struct Level_params
{
    sf::Vector2i tile_map_chunk_size;
    Gen_params gen_params;
};