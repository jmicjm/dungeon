#pragma once
#include "../level/tile_sprite_id_enum.h"
#include "../gfx//tile_sprite_data.h"

#include <SFML/Graphics/Texture.hpp>

#include <map>
#include <vector>


class tile_sprite_storage
{
	using tile_sprite_map = std::map<TILE_SPRITE_ID::tile_sprite_id_t, std::vector<tile_sprite_data>>;
	static tile_sprite_map sprite_map;

	static void expandTile(std::pair<const TILE_SPRITE_ID::tile_sprite_id_t, std::vector<tile_sprite_data>>& i);
	static void expandTileSet();

public:
	static void loadSprites();
	static std::vector<tile_sprite_data>* getSprite(TILE_SPRITE_ID::tile_sprite_id_t key);
};