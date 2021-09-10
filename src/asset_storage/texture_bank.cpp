#include "texture_bank.h"

#include <string_view>


std::map<std::string, sf::Texture> Texture_bank::texture_map = std::map<std::string, sf::Texture>();

const sf::Texture* Texture_bank::getTexture(const std::string& key)
{
    auto it = texture_map.find(key);
    if (it != texture_map.end())
    {
        return &it->second; 
    }
    else
    {
        sf::Texture tex;
        tex.loadFromFile(key);
        texture_map.insert({ key, tex });

        return &texture_map[key];
    }
}