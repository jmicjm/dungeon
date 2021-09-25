#pragma once
#include "../level/tile_sprite_id_enum.h"
#include "../gfx/primitive_sprite.h"
#include "texture_bank.h"

#include <map>
#include <vector>


class Tile_sprite_storage
{
    struct Sprite_chance
    {
        Primitive_sprite sprite;
        float chance;
    };

    using Tile_sprite_map = std::map<TILE_SPRITE_ID::tile_sprite_id_t, std::vector<Sprite_chance>>;
    static Tile_sprite_map createSpriteMap();
    inline static Tile_sprite_map sprite_map = createSpriteMap();

public:    
    static std::vector<Sprite_chance>* getSprite(TILE_SPRITE_ID::tile_sprite_id_t key);
};