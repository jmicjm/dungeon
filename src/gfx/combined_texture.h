#pragma once
#include "SFML/Graphics/Texture.hpp"

#include <unordered_map>

class Combined_texture
{
    sf::Texture combined_texture;
    std::unordered_map<const sf::Texture*, sf::Vector2f> texture_offset_map;

public:
    sf::Vector2f getTextureOffset(const sf::Texture* texture);
    const sf::Texture& getTexture() const;
};