#pragma once
#include "../components/player.h"
#include "../world/world.fwd.h"
#include "../utils/quadtree.h"

#include <entt/entt.hpp>


entt::entity createPlayer(entt::registry& registry);
bool updatePlayer(entt::registry& registry, Quadtree<entt::entity>& entities, World& world, Player& player);
