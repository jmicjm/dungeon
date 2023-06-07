#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "inventory.h"
#include "body_inventory.h"
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
        Body_inventory body_inventory_a;
        Body_inventory body_inventory_b;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

        void updateInventorySizes();

    public:
        Dual_inventory(World& world, entt::entity entity_a, entt::entity entity_b);

        void update() override;
    };
}