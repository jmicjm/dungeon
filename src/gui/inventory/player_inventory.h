#pragma once
#include "../gui_component.h"
#include "../../world/world.fwd.h"
#include "../primitives/frame/frame.h"
#include "../primitives/text/text.h";
#include "../primitives/button/button.h"
#include "inventory.h"
#include "item_swapper.h"
#include "../../components/inventory.h"
#include <entt/entt.hpp>


namespace gui
{
    class Player_inventory : public Gui_component
    {
        World& world;
        entt::entity player;

        Frame bg;
        Inventory inventory;
        Item_swapper item_swapper;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Player_inventory(World& world, entt::entity player);

        void update() override;
    };
}
