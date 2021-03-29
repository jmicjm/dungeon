#pragma once
#include <cstdint>

namespace TILE_SPRITE_ID
{
	using tile_sprite_id_t = uint32_t;

	enum TILE_SPRITE_ID : tile_sprite_id_t
	{
		/*
	    sorrounding

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
		L  = 1,

		//type
		ROCK = 1 << 8,
		WOOD = 2 << 8,
		//WOOD2 = 3 << 8 x, etc

		//tile type
		WALL    = 1 << 31,
		FLOOR   = 1 << 30,
		HALLWAY = 1 << 29
	};
}