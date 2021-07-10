#include "level_tile_map_chunk.h"
#include "../gfx/tile_sprite_data.h"
#include "../asset_storage/tile_sprite_storage.h"

#include <algorithm>

level_tile_map_chunk::texture_vertices<sf::VertexArray>& level_tile_map_chunk::getVertexArray(const sf::Texture* texture, vertex_arrays_type& vertex_arrays)
{
	auto it = std::find_if(
		vertex_arrays.begin(),
		vertex_arrays.end(),
		[&](const texture_vertices<sf::VertexArray>& tva) {return tva.texture == texture; }
	);
	if (it != vertex_arrays.end())
	{
		return *it;
	}
	else
	{
		vertex_arrays.push_back(texture);
		return vertex_arrays.back();
	}
}

void level_tile_map_chunk::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	for (auto& i : vertex_buffers)
	{
		st.texture = i.texture;
		rt.draw(i.vertices, st);
	}
}

void level_tile_map_chunk::copyToBuffer(const vertex_arrays_type& vertex_arrays)
{
	vertex_buffers.resize(vertex_arrays.size());

	for (int i = 0; i < vertex_buffers.size(); i++)
	{
		vertex_buffers[i].texture = vertex_arrays[i].texture;
		vertex_buffers[i].vertices.setPrimitiveType(sf::Quads);
		vertex_buffers[i].vertices.create(vertex_arrays[i].vertices.getVertexCount());
		vertex_buffers[i].vertices.update(&(vertex_arrays[i].vertices[0]));
	}
}

void level_tile_map_chunk::populate(const level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area)
{
	vertex_arrays_type vertex_arrays;

	for (int x = area.left; x < ls.getSize().x && x < area.left + area.width; x++)
	{
		for (int y = area.top; y < ls.getSize().y && y < area.top + area.height; y++)
		{
			for (const auto& sprite_data : ls.at({ x,y }).sprites_info)
			{
				const tile_sprite_data& tsd = tile_sprite_storage::getSprite(sprite_data.id)->at(sprite_data.id_variant);

				texture_vertices<sf::VertexArray>& va = getVertexArray(tsd.texture, vertex_arrays);

				for (auto vertex : tsd.vertices)
				{
					vertex.position += {tile_size.x * x, tile_size.y * y};

					va.vertices.append(vertex);
				}

				if (sprite_data.id & TILE_SPRITE_ID::OVERLAY)//put overlays on top
				{
					std::swap(vertex_arrays.back(), va);
				}
			}
		}
	}
	copyToBuffer(vertex_arrays);
}