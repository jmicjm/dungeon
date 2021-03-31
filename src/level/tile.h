#pragma once
#include "tile_sprite_id_enum.h"
#include "SFML/Graphics/Sprite.hpp"


enum class TILE_TYPE
{
	WALL, ROOM, HALLWAY
};

struct tile
{
	TILE_TYPE type = TILE_TYPE::WALL;

	TILE_SPRITE_ID::tile_sprite_id_t id;
	unsigned int id_variant;

	sf::Sprite* sprite;
	/*
	bool revealed;
	*/
};