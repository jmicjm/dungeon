#pragma once
#include "../gui_component.h"
#include "../component_stack/component_stack_helper.h"
#include "../primitives/frame/frame.h"
#include "inventory.h"
#include "../../components/inventory.h"
#include <entt/entt.hpp>


namespace gui
{
    class Dual_inventory : public Gui_component, public Component_stack_helper
    {
        Frame bg;
        Inventory inventory_a;
        Inventory inventory_b;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Dual_inventory(entt::registry& registry, entt::entity entity_a, entt::entity entity_b);

        void update() override;
    };
}