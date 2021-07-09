#include "level_tile_map.h"
#include "../gfx/tile_sprite_data.h"
#include "../asset_storage/tile_sprite_storage.h"

#include <algorithm>

level_tile_map::texture_vertex_array& level_tile_map::getVertexArray(const sf::Texture* texture)
{
	auto it = std::find_if(
		vertex_arrays.begin(),
		vertex_arrays.end(),
		[&](const texture_vertex_array& tva) {return tva.texture == texture; }
	);
	if (it != vertex_arrays.end()) 
	{
		return *it; 
	}
	else
	{
		vertex_arrays.push_back(texture);
		vertex_arrays.back().vertices.setPrimitiveType(sf::Quads);
		return vertex_arrays.back();
	}
}

void level_tile_map::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	for (auto& i : vertex_arrays)
	{
		st.texture = i.texture;
		rt.draw(i.vertices, st);
	}
}

void level_tile_map::populate(const level_structure& ls, const sf::Vector2f& tile_size)
{
	vertex_arrays.clear();

	for (int x = 0; x < ls.getSize().x; x++)
	{
		for (int y = 0; y < ls.getSize().y; y++)
		{
			for (const auto& sprite_data : ls.at({ x,y }).sprites_info)
			{
				const tile_sprite_data& tsd = tile_sprite_storage::getSprite(sprite_data.id)->at(sprite_data.id_variant);

				texture_vertex_array& va = getVertexArray(tsd.texture);

				if (sprite_data.id & TILE_SPRITE_ID::OVERLAY)//put overlays on top
				{
					std::swap(vertex_arrays.back(), va);
				}

				for (auto vertex : tsd.vertices)
				{
					vertex.position += sf::Vector2f(tile_size.x*x, tile_size.y*y);

					va.vertices.append(vertex);
				}
			}
		}
	}
}
