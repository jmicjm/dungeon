#pragma once
#include "../../components/inventory.h"
#include "item_swapper.h"
#include "../../components/body/body.h"
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../primitives/list/list.h"
#include "../../world/world.fwd.h"
#include <entt/entt.hpp>


namespace gui
{
    class Body_inventory : public Gui_component
    {
        World& world;
        entt::entity entity;
        Item_swapper& item_swapper;

        List list;


        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;


        void fillList(Item_swapper& item_swapper);
        std::vector<Body_part*> bodyPartsWithInventory();
        std::unique_ptr<Gui_component> entryFromBodyPart(const Body_part& bp);
        Body* getBody();

    public:
        Body_inventory(World& world, entt::entity entity, Item_swapper& item_swapper);

        void update() override;
    };
}