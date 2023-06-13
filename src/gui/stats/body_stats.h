#pragma once
#include "../gui_component.h"
#include "../primitives/text/text.h"
#include "entt/entt.hpp"
#include <string>


namespace gui
{
    class Body_stats : public Gui_component
    {
        const entt::registry& registry;
        const entt::entity entity;

        Text stats;

        void redraw() override;

        void activateEvent() override;
        void deactivateEvent() override;

        std::string generateText();

    public:
        Body_stats(const entt::registry& registry, const entt::entity entity);

        void update() override;
    };
}