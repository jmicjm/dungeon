#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "level_params.h"
#include "door_controller.h"
#include "../entities/entity.fwd.h"
#include "../utils/quadtree.h"

#include <memory>

class Level
{
public:
    Level_structure structure;
    Level_tile_map tile_map;
    Tile_reveal_mask reveal_mask;
    sf::Vector2i tile_size;
    Quadtree<std::shared_ptr<Entity>> entities;
    Door_controller door_controller{this};


    void create(const Level_params& params);
};