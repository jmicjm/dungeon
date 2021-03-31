#pragma once
#include "../level/tile_sprite_id_enum.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <map>
#include <vector>


class tile_sprite_storage
{
	using tile_sprite_map = std::map<TILE_SPRITE_ID::tile_sprite_id_t, std::vector<sf::Sprite>>;
	static tile_sprite_map sprite_map;

	static void expandTile(std::pair<const TILE_SPRITE_ID::tile_sprite_id_t, std::vector<sf::Sprite>>& i);
	static void expandTileSet();

public:
	static void loadSprites();
	static std::vector<sf::Sprite>* getSprite(TILE_SPRITE_ID::tile_sprite_id_t key);
};