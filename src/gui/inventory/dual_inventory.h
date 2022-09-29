#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "inventory.h"
#include "item_swapper.h"
#include "../../components/inventory.h"
#include "../../world/world.fwd.h"
#include <entt/entt.hpp>


namespace gui
{
    class Dual_inventory : public Gui_component
    {
        Frame bg;
        Inventory inventory_a;
        Inventory inventory_b;
        Item_swapper item_swapper;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Dual_inventory(World& world, entt::entity entity_a, entt::entity entity_b);

        void update() override;
    };
}