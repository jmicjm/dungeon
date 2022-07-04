#pragma once
#include "level.h"
#include "../components/position.h"

#include "SFML/System/Vector2.hpp"


void moveEntity(Quadtree<entt::entity>& entities, Position& position, sf::Vector2i& offset);
