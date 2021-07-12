#include "tile_sprite_storage.h"
#include "texture_bank.h"

#include "SFML/Graphics/Rect.hpp"

#include <string>
#include <iostream>

Tile_sprite_storage::Tile_sprite_map Tile_sprite_storage::sprite_map = Tile_sprite_map();

void Tile_sprite_storage::expandTile(std::pair<const TILE_SPRITE_ID::tile_sprite_id_t, std::vector<Tile_sprite_data>>& i)
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

void Tile_sprite_storage::expandTileSet()
{	
	for (auto& i : sprite_map)
	{
		expandTile(i);
	}
}

void Tile_sprite_storage::loadSprites()
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
		std::vector<Tile_sprite_data> tmp_spr_vec;

		for (auto& j : i.variants)
		{
			const sf::Texture* tex_ptr = Texture_bank::getTexture(j.path);
			Tile_sprite_data sprite_data(tex_ptr, j.rect);
			tmp_spr_vec.push_back(sprite_data);
		}
		sprite_map.insert({ i.tile_type_id, tmp_spr_vec });
	}

	expandTileSet();
}

std::vector<Tile_sprite_data>* Tile_sprite_storage::getSprite(TILE_SPRITE_ID::tile_sprite_id_t key)
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
