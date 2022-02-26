#pragma once
#include "../../asset_storage/tile_sprite_storage.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/View.hpp"
#include <utility>

//returns pair of top-left and bottom-right visible tiles
std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBoundsTiles(const sf::View& view, const sf::Vector2i& tile_size = Tile_sprite_storage::tile_size);

//returns pair of top-left and bottom-right visible pixels
std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBoundsPixels(const sf::View& view);
