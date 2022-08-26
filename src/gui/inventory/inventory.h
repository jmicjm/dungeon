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

        int slotsPerRow(bool with_scroll = true) const;
        sf::Vector2f slotToCoords(unsigned int slot) const;

    public:
        Inventory(entt::registry& registry, entt::entity entity);

        const ::Inventory* inventory() const;
        ::Inventory* inventory();

        std::tuple<bool, unsigned int, sf::Vector2f> coordsToSlot(const sf::Vector2f& coords) const;

        void update() override;
    };
}