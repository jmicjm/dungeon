#pragma once
#include "../level/level_structure.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"


class level_structure_decorator
{
    level_structure* ls = nullptr;

    TILE_SPRITE_ID::tile_sprite_id_t getTileSpriteId(const vec2i pos);
   // TILE_SPRITE_ID::tile_sprite_id_t get...Id(const vec2i pos, const std::vector<vec2i>& pattern);

    bool addSprite(const vec2i pos, TILE_SPRITE_ID::tile_sprite_id_t id);

    void placeCarpet(const rect_i area);

public:
    void decorate(level_structure& l);
};