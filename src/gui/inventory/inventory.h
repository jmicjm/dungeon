#pragma once
#include "../../components/inventory.h"
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../primitives/scroll/scroll.h"
#include <entt/entt.hpp>


namespace gui
{
    class Inventory : public Gui_component
    {
        entt::registry& registry;
        entt::entity entity;

        Frame item_field;
        Scroll scroll;

        int item_field_size = 64;
        int item_field_border = 2;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;
        void resizeEvent(sf::Vector2f size_diff) override;

        int slotsPerRow() const;
        sf::Vector2f slotToCoords(unsigned int slot) const;
        const ::Inventory* inventory() const;

    public:
        Inventory(entt::registry& registry, entt::entity entity);

        void update() override;
    };
}