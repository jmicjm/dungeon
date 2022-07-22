#pragma once
#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <unordered_map>


//uses file path as key
class Texture_bank
{
    static std::unordered_map<std::string, sf::Texture> initialize_texture_map();

    static sf::Texture fallback_texture;

    inline static std::unordered_map<std::string, sf::Texture> texture_map = initialize_texture_map();

public:
    static const sf::Texture* getTexture(const std::string& tex_path);
};