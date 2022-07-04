#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../primitives/button/button.h"
#include "../../entities/player.h"

#include <entt/entt.hpp>


namespace gui 
{
    class Controls : public Gui_component
    {
        Button options_button;
        Button skip_button;
        Button center_button;

        float scale = 2.f;
        void rescale();

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;
    public:
        Controls();

        void update() override;

        void setPlayer(const entt::registry& registry, entt::entity player);
        void setScale(float scale);
        float getScale() const;
    };
}