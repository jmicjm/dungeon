#pragma once
#include "../level/tile_sprite_id_enum.h"
#include "../gfx/primitive_sprite.h"

#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <vector>


class Tile_sprite_storage
{
    using Tile_sprite_map = std::map<TILE_SPRITE_ID::tile_sprite_id_t, std::vector<Primitive_sprite>>;
    static Tile_sprite_map sprite_map;

public:
    static void loadSprites();
    static std::vector<Primitive_sprite>* getSprite(TILE_SPRITE_ID::tile_sprite_id_t key);
};