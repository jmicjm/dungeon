#pragma once
#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <unordered_map>


//uses file path as key
class Texture_bank
{
    inline static std::unordered_map<std::string, sf::Texture> texture_map;

public:
    /**
    returns pointer to texture identified by tex_path,
    if there is no given texture loaded tries to load it from file
    */
    static const sf::Texture* getTexture(const std::string& tex_path);
};