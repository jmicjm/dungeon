#pragma once
#include "../../level/level_structure.h"
#include "level_tile_map_texture.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


class Level_tile_map_chunk : public sf::Drawable
{
	Level_tile_map_texture* texture = nullptr;
	sf::VertexBuffer vertices;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

	void copyToBuffer(const sf::VertexArray& vertex_array);

public:
	void populate(const Level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area);
	void setTexture(Level_tile_map_texture* tex);
};