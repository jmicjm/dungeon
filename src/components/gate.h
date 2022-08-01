#pragma once
#include "render_component.h"
#include "entt/entt.hpp"


struct Gate
{
    Render_component open_rc;
    Render_component closed_rc;

    enum STATE : uint8_t
    {
        OPEN = 1,
        CLOSED = 1 << 1
    };
    STATE passable_states;
    STATE opaque_states;

    bool is_open;
};

struct Gate_moving_from
{
    entt::entity moving_from_entity = entt::null;
};