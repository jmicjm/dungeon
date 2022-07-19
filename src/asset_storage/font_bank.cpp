#include "font_bank.h"
#include "../generated_assets/fonts.h"

std::unordered_map<std::string, sf::Font> Font_bank::initialize_font_map()
{
    using namespace FONTS_ASSETS;

    std::unordered_map<std::string, sf::Font> map;

    for(int i=0; i<ASSETS_COUNT; i++){
        sf::Font font;
        font.loadFromMemory(ASSETS_DATA[i], ASSET_SIZES[i]);
        map.insert({ std::string(ASSET_PATHS[i]), font });
    }

    return map;
}


const sf::Font* Font_bank::getFont(const std::string& key)
{
    auto it = font_map.find("assets/fonts/" + key);
    if(it == font_map.end()) {
        return &fallback_font;
    }
    return &it->second;
}