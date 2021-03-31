#include "tile_sprite_storage.h"
#include "texture_bank.h"

#include "SFML/Graphics/Rect.hpp"

#include <string>
#include <iostream>

tile_sprite_storage::tile_sprite_map tile_sprite_storage::sprite_map = tile_sprite_map();

void tile_sprite_storage::expandTile(std::pair<const TILE_SPRITE_ID::tile_sprite_id_t, std::vector<sf::Sprite>>& i)
{
	using namespace TILE_SPRITE_ID;

	const tile_sprite_id_t id = i.first;
	if (id & (TL|TR|BR|BL))
	{
		//expansion of conflicting tiles
		for (int j = 0; j < 16; j++)
		{
			const tile_sprite_id_t id2 = id | j << 4;
			if (id2 != id)
			{
				auto it = sprite_map.find(id2);
				if (it != sprite_map.end())
				{
					expandTile(*it);
				}
			}
		}

		for (int j = 0; j < 16; j++)
		{
			const tile_sprite_id_t id2 = id | j << 4;
			if (id2 != id)
			{
				auto it = sprite_map.find(id2);
				if (it == sprite_map.end())
				{
					sprite_map.insert({ id2, i.second });
				}
			}
		}
	}
}

void tile_sprite_storage::expandTileSet()
{	
	for (auto& i : sprite_map)
	{
		expandTile(i);
	}
}

void tile_sprite_storage::loadSprites()
{
	using namespace TILE_SPRITE_ID;
	struct s_data
	{
		struct spr { std::string path; sf::IntRect rect; };
		tile_sprite_id_t tile_type_id;
		std::vector<spr> variants;
	};
	static std::vector<s_data> data =
	{
		#include "tile_sprite_list.h"
	};

	for (auto& i : data)
	{
		std::vector<sf::Sprite> tmp_spr_vec;

		for (auto& j : i.variants)
		{
			const sf::Texture* tex_ptr = texture_bank::getTexture(j.path);
			sf::Sprite sprite;
			sprite.setTexture(*tex_ptr);
			sprite.setTextureRect(j.rect);
			tmp_spr_vec.push_back(sprite);
		}
		sprite_map.insert({ i.tile_type_id, tmp_spr_vec });
	}

	expandTileSet();
}

std::vector<sf::Sprite>* tile_sprite_storage::getSprite(TILE_SPRITE_ID::tile_sprite_id_t key)
{
	auto it = sprite_map.find(key);
	if (it != sprite_map.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}
