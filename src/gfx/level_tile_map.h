#pragma once
#include "../level/level_structure.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <vector>


class level_tile_map : public sf::Drawable
{
	struct texture_vertex_array
	{
		const sf::Texture* texture;
		sf::VertexArray vertices;

		texture_vertex_array(const sf::Texture* texture) : texture(texture) {}
	};

	std::vector<texture_vertex_array> vertex_arrays;

	sf::VertexArray& getVertexArray(const sf::Texture* texture);

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
	
public:
	void populate(const level_structure& ls, const sf::Vector2f& tile_size);
};