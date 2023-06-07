#pragma once
#include "../../../world/world.h"
#include "entt/entt.hpp"


entt::entity createInfectedZombie(entt::registry& registry);
bool updateInfectedZombie(entt::registry& registry, World& world, const entt::entity entity);