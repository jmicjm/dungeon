#include "tile_sprite_storage.h"
#include "texture_bank.h"

#include "SFML/Graphics/Rect.hpp"

#include <string>


Tile_sprite_storage::Tile_sprite_map Tile_sprite_storage::sprite_map = Tile_sprite_map();

void Tile_sprite_storage::loadSprites()
{
	using namespace TILE_SPRITE_ID;
	struct s_data
	{
		struct spr { std::string path; sf::IntRect rect; sf::Vector2f offset = { 0,0 }; };
		tile_sprite_id_t tile_type_id;
		std::vector<spr> variants;
	};
	static std::vector<s_data> data =
	{
		#include "tile_sprite_list.h"
	};

	for (auto& i : data)
	{
		std::vector<Primitive_sprite> tmp_spr_vec;

		for (auto& j : i.variants)
		{
			const sf::Texture* tex_ptr = Texture_bank::getTexture(j.path);
			Primitive_sprite sprite(tex_ptr, j.rect);
			sprite.move(j.offset);
			tmp_spr_vec.push_back(sprite);
		}
		sprite_map.insert({ i.tile_type_id, tmp_spr_vec });
	}
}

std::vector<Primitive_sprite>* Tile_sprite_storage::getSprite(TILE_SPRITE_ID::tile_sprite_id_t key)
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
