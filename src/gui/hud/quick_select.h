#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include <entt/entt.hpp>


namespace gui
{
    class Quick_select : public Gui_component
    {
        entt::registry& registry;
        entt::entity player;

        Frame bg;
        Frame fg;

        float scale = 3;

        void redraw() override;

    public:
        Quick_select(entt::registry& registry, entt::entity player);

        void setPlayer(entt::entity player);

        void setScale(float scale);
    };
}