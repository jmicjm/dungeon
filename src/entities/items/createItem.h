#pragma once
#include "item_ids.h"
#include "entt/entt.hpp"

namespace items
{
    entt::entity createItem(entt::registry& registry, const Item_id id);
}