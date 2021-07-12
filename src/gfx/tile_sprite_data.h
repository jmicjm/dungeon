#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

struct tile_sprite_data
{
	const sf::Texture* texture = nullptr;

	sf::Vertex vertices[4];

	tile_sprite_data(const sf::Texture* texture, const sf::IntRect& rect);
};