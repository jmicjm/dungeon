#pragma once
#include "../gui_component.h"
#include "../component_stack/component_stack_helper.h"
#include "../primitives/frame/frame.h"
#include "../primitives/text/text.h";
#include "../primitives/button/button.h"
#include "inventory.h"
#include "../../components/inventory.h"
#include <entt/entt.hpp>


namespace gui
{
    class Player_inventory : public Gui_component, public Component_stack_helper
    {
        entt::registry& registry;
        entt::entity player;

        Frame bg;
        Inventory inventory;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Player_inventory(entt::registry& registry, entt::entity player);

        void update() override;
    };
}
