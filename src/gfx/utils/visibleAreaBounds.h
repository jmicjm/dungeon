#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/View.hpp"
#include <utility>

//returns pair of top-left and bottom-right visible tiles
std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBounds(const sf::View& view, const sf::Vector2i& tile_size);

//returns pair of top-left and bottom-right visible pixels
std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBounds(const sf::View& view);
