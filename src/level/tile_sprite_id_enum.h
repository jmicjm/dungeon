#pragma once
#include <cstdint>

namespace TILE_SPRITE_ID
{
	using tile_sprite_id_t = uint32_t;

	enum SURROUNDINGS : tile_sprite_id_t
	{
		/*
		   | T |   
		---|---|---
		L  | X |  R
		---|---|---
		   | B |   
		*/
		T = 1 << 3,
		R = 1 << 2,
		B = 1 << 1,
		L = 1
	};

	enum TILE_SUBTYPE : tile_sprite_id_t
	{
		ROCK = 1 << 4,
		WOOD = 2 << 4
	};

	enum TILE_TYPE : tile_sprite_id_t
	{
		WALL    = 1 << 28,
		FLOOR   = 2 << 28,
		HALLWAY = 3 << 28
	};
}