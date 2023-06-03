#pragma once
#include "../../../world/world.h"
#include "entt/entt.hpp"


entt::entity createSmuggler(entt::registry& registry);
bool updateSmuggler(entt::registry& registry, World& world, const entt::entity entity);