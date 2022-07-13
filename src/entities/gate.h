#pragma once
#include "../components/gate.h"
#include "../utils/quadtree.h"

#include <entt/entt.hpp>


bool openGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate);
bool closeGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate);