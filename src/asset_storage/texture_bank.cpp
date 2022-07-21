#include "texture_bank.h"
#include "../generated_assets/textures.h"
#include <iostream>

using namespace TEXTURES_ASSETS;

sf::Texture Texture_bank::fallback_texture = []{sf::Texture tex; tex.loadFromMemory(ASSETS_DATA[ASSETS_COUNT-1], ASSET_SIZES[ASSETS_COUNT-1]); return tex;}();

std::unordered_map<std::string, sf::Texture> Texture_bank::initialize_texture_map()
{
    std::unordered_map<std::string, sf::Texture> map;

    for(int i=0; i<ASSETS_COUNT; i++){
        sf::Texture tex;
        tex.loadFromMemory(ASSETS_DATA[i], ASSET_SIZES[i]);
        map.insert({ std::string(ASSET_PATHS[i]), tex });
    }

    return map;
}


const sf::Texture* Texture_bank::getTexture(const std::string& key)
{
    auto it = texture_map.find("assets/textures/" + key);
    if(it == texture_map.end()) {
        std::cout << "Texture: " << key << " not found, using fallback texture" << std::endl;
        return &fallback_texture;
    }
    return &it->second;
}