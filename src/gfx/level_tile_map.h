#pragma once
#include "level_tile_map_chunk.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>


class level_tile_map : public sf::Drawable
{
	std::vector<std::vector<level_tile_map_chunk>> chunks;
	sf::Vector2i chunk_size_px;

	level_tile_map_chunk& at(const sf::Vector2i& pos);
	const level_tile_map_chunk& at(const sf::Vector2i& pos) const;
	void setSize(const sf::Vector2i& size);
	sf::Vector2i getSize() const;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
	void populate(const level_structure& ls, const sf::Vector2f& tile_size, const sf::Vector2i& chunk_size);
};