#pragma once
#include "level_tile_map_chunk.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>


class Level_tile_map : public sf::Drawable
{
	std::vector<std::vector<Level_tile_map_chunk>> chunks;
	sf::Vector2i chunk_size_px;

	Level_tile_map_chunk& at(const sf::Vector2i& pos);
	const Level_tile_map_chunk& at(const sf::Vector2i& pos) const;
	void setSize(const sf::Vector2i& size);
	sf::Vector2i getSize() const;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
	void populate(const Level_structure& ls, const sf::Vector2f& Tile_size, const sf::Vector2i& chunk_size);
};