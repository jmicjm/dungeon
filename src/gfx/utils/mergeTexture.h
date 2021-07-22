#pragma once
#include "SFML/Graphics/Texture.hpp"

/**merges texture @b into texture @a, returns offset of merged texture @b, position of texture @a contents remains unchanged*/
sf::Vector2f mergeTexture(sf::Texture& a, const sf::Texture& b);