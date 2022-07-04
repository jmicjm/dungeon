#pragma once
#include "tile_visibility_info.h"
#include "level.h"
#include "../utils/quadtree.h"
#include "../utils/sf_vector2_hash.h"
#include "SFML/System/Vector2.hpp"

#include <entt/entt.hpp>

#include <unordered_map>


std::unordered_map<sf::Vector2i, Tile_visibility_info> visibleTiles(const sf::Vector2i& position, const Level& level, const Quadtree<entt::entity>& entities);