#include "dual_inventory.h"


void gui::Dual_inventory::redraw()
{
    bg.draw();
    inventory_a.draw();
    inventory_b.draw();
}

void gui::Dual_inventory::activateEvent()
{
    inventory_a.activate();
    inventory_b.activate();
}

void gui::Dual_inventory::deactivateEvent()
{
    inventory_a.deactivate();
    inventory_b.deactivate();
}

gui::Dual_inventory::Dual_inventory(entt::registry& registry, entt::entity entity_a, entt::entity entity_b)
    : inventory_a(registry, entity_a), inventory_b(registry, entity_b)
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });
    bg.setAppearance({ rect({41, 32, 22, 224}) });

    inventory_a.setParent(this);
    inventory_a.setSizeInfo({ .percentage = {0.5, 1} });

    inventory_b.setParent(this);
    inventory_b.setSizeInfo({ .percentage = {0.5, 1} });
    inventory_b.setPositionInfo({ .relative_to = {1,0} });
}

void gui::Dual_inventory::update()
{
    inventory_a.update();
    inventory_b.update();
}
