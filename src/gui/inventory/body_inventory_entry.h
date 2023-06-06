#pragma once
#include "inventory.h"
#include "item_swapper.h"
#include "../primitives/text/text.h"
#include "../../components/inventory.h"
#include "../gui_component.h"
#include "../../world/world.fwd.h"
#include <entt/entt.hpp>


namespace gui
{
    class Body_inventory_entry : public Gui_component
    {
        Inventory inventory;
        Text description;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Body_inventory_entry(World& world, entt::entity entity, const std::string& desc, Item_swapper& item_swapper);

        void update() override;
    };
}