#pragma once
#include "../level/level_structure.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"

#include <vector>


class level_structure_decorator
{
    level_structure* ls = nullptr;

    TILE_SPRITE_ID::tile_sprite_id_t getTileSpriteId(const vec2i pos);

public:
    level_structure_decorator() {}
    level_structure_decorator(level_structure& ls) : ls(&ls) {}

    void setLevelStructure(level_structure& l);

    void decorate();
};