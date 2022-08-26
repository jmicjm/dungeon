#pragma once
#include "../gui_component.h"
#include "inventory.h"
#include "../../components/inventory.h"
#include <entt/entt.hpp>
#include <vector>
#include <functional>


namespace gui
{
    class Item_swapper : public Gui_component
    {
        entt::registry& registry;
        std::vector<std::reference_wrapper<Inventory>> inventories;
        Inventory* src_inventory = nullptr;
        unsigned int src_slot;
        entt::entity hold_item = entt::null;
        sf::Vector2f hold_offset;

        void redraw() override;
        void deactivateEvent() override;

        void restore();

    public:
        Item_swapper(entt::registry& registry, std::vector<std::reference_wrapper<Inventory>> inventories);
        ~Item_swapper();

        void update() override;
    };
}
