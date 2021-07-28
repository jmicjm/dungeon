#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "level_params.h"

class Level
{
public:
    Level_structure ls;
    Level_tile_map tmap;
    Tile_reveal_mask reveal_mask;
    //entities
    //doors
    //etc

    void create(const Level_params& params);
};