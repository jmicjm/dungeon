#pragma once
#include "level.h"
#include "../components/position.h"

#include <entt/entt.hpp>

#include "SFML/System/Vector2.hpp"


void moveEntity(entt::registry& registry, Quadtree<entt::entity>& entities, Position& position, sf::Vector2i offset);
