#include "texture_bank.h"


const sf::Texture* Texture_bank::getTexture(const std::string& key)
{  
    if (auto it = texture_map.find(key); it != texture_map.end())
    {
        return &it->second; 
    }
    else
    {
        sf::Texture tex;
        tex.loadFromFile(key);
        return &texture_map.insert({ key, tex }).first->second;
    }
}