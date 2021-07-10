#pragma once
#include "../level/level_structure.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>


class level_tile_map_chunk : public sf::Drawable
{
	template<typename T>
	struct texture_vertices
	{
		const sf::Texture* texture = nullptr;
		T vertices;

		texture_vertices<T>() {}
		texture_vertices<T>(const sf::Texture* texture) : texture(texture) {}
	};

	using vertex_arrays_type  = std::vector<texture_vertices<sf::VertexArray>>;
	using vertex_buffers_type = std::vector<texture_vertices<sf::VertexBuffer>>;

	vertex_buffers_type vertex_buffers;

	texture_vertices<sf::VertexArray>& getVertexArray(const sf::Texture* texture, vertex_arrays_type& vertex_arrays);

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

	void copyToBuffer(const vertex_arrays_type& vertex_arrays);

public:
	void populate(const level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area);
};