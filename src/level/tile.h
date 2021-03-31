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

	TILE_SPRITE_ID::tile_sprite_id_t id = 0;
	unsigned int id_variant = 0;

	sf::Sprite* sprite = nullptr;
	/*
	bool revealed;
	*/
};