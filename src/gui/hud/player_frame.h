#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../../entities/player.h"

#include <entt/entt.hpp>


namespace gui
{
    class Player_frame : public Gui_component
    {
        entt::entity player;
        const entt::registry* registry = nullptr;
        Frame frame;

        void redraw() override;

    public:
        Player_frame();

        void setPlayer(const entt::registry& registry, entt::entity player);
        void setAppearance(const Frame_appearance& appearance);
    };
}
