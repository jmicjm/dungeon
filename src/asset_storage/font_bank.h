#pragma once
#include <SFML/Graphics/Font.hpp>

#include <string>
#include <unordered_map>


//uses file path as key
class Font_bank
{
    static std::unordered_map<std::string, sf::Font> initialize_font_map();

    inline static sf::Font fallback_font = sf::Font();

    inline static std::unordered_map<std::string, sf::Font> font_map = initialize_font_map();

public:
    static const sf::Font* getFont(const std::string& font_path);
};