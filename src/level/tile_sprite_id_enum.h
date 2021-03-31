#pragma once
#include <cstdint>

namespace TILE_SPRITE_ID
{
	using tile_sprite_id_t = uint32_t;

	enum TILE_SPRITE_SURROUNDINGS : tile_sprite_id_t
	{
		/*
		TL | T | TR  
		---|---|---
		L  | X |  R
		---|---|---
		BL | B | BR  
		*/
		TL = 1 << 7,
		T  = 1 << 6,
		TR = 1 << 5,
		R  = 1 << 4,
		BR = 1 << 3,
		B  = 1 << 2,
		BL = 1 << 1,
		L  = 1
	};

	enum TILE_SPRITE_SUBTYPE : tile_sprite_id_t
	{
		ROCK = 1 << 8,
		WOOD = 2 << 8
	};

	enum TILE_SPRITE_TYPE : tile_sprite_id_t
	{
		WALL    = 1 << 28,
		FLOOR   = 2 << 28,
		HALLWAY = 3 << 28
	};
}