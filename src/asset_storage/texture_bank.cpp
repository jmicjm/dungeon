#include "texture_bank.h"
#include "../generated_assets/textures.h"

std::unordered_map<std::string, sf::Texture> Texture_bank::initialize_texture_map()
{
    using namespace TEXTURES_ASSETS;

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
    auto it = texture_map.find(key);
    if(it == texture_map.end()) {
        return &fallback_texture;
    }
    return &it->second;
}