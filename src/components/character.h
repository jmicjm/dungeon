#pragma once
#include "../world/world.h"

struct Character 
{
    using update_f = bool(entt::registry&, World&, const entt::entity);

    update_f* update = nullptr;
};