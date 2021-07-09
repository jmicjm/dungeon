#pragma once
#include "tile_sprite_id_enum.h"
#include "SFML/Graphics/Sprite.hpp"

#include <vector>


enum class TILE_TYPE
{
	WALL, ROOM, HALLWAY, DOORWAY
};

struct tile
{
	TILE_TYPE type = TILE_TYPE::WALL;

	struct sprite_info
	{
		TILE_SPRITE_ID::tile_sprite_id_t id;
		unsigned int id_variant;

		sprite_info(TILE_SPRITE_ID::tile_sprite_id_t id = 0, unsigned int id_variant = 0)
			: id(id), id_variant(id_variant) {}
	};
	std::vector<sprite_info> sprites_info;
};