#pragma once
#include "../../components/body/body.h"
#include <entt/entt.hpp>
#include <string>


Body createHumanoidBody(entt::registry& registry);
Body_part createArm(entt::registry& registry, const std::string& desc);
Body_part createLeg(entt::registry& registry, const std::string& desc);
