#pragma once
#include "../gui_component.h"
#include "../primitives/button/button.h"
#include "stat_bar.h"
#include "player_frame.h"
#include "quick_select.h"
#include "controls.h"
#include "../../entities/player.h"

#include <entt/entt.hpp>


namespace gui
{
    class Hud : public Gui_component
    {
        entt::entity player;
        const entt::registry* registry = nullptr;

        Player_frame player_frame;
        Stat_bar hp_bar;
        Stat_bar mana_bar;
        Quick_select quick_select;
        Button inventory_button;
        Controls controls;

        float scale = 3;

        void rescale();

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Hud();

        void update() override;

        void setPlayer(entt::registry& registry, entt::entity player);
        void setScale(float scale);
        float getScale() const;
    };
}