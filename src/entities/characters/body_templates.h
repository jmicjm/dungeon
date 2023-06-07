#pragma once
#include "../../components/body/body.h"
#include "../../components/inventory.h"
#include <entt/entt.hpp>
#include <string>


Body createHumanoidBody(entt::registry& registry);
Body_part createArm(entt::registry& registry, const std::string& desc);
Body_part createLeg(entt::registry& registry, const std::string& desc);

Inventory createHeadInventory();
Inventory createUpperTorsoInventory();
Inventory createLowerTorsoInventory();
Inventory createFootInventory();
Inventory createHandInventory();
