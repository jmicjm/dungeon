#pragma once
#include "SFML/Graphics/Texture.hpp"

#include <map>

class Level_tile_map_texture
{
	sf::Texture combined_texture;
	std::map<const sf::Texture*, const sf::Vector2f> texture_offset_map;

public:
	sf::Vector2f getTextureOffset(const sf::Texture* texture);
	const sf::Texture& getCombinedTexture() const;
};