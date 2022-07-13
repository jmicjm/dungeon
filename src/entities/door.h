#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"

#include <entt/entt.hpp>


entt::entity createDoorFront(entt::registry& registry);
entt::entity createDoorSide(entt::registry& registry);
entt::entity createDoor(entt::registry& registry, Animated_sprite open_rc, Animated_sprite closed_rc);
