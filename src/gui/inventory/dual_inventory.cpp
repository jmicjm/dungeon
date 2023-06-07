#include "dual_inventory.h"
#include "../../world/world.h"


void gui::Dual_inventory::redraw()
{
    bg.draw();
    if (inventory_a.inventory()) inventory_a.draw();
    if (inventory_b.inventory()) inventory_b.draw();
    if (body_inventory_a.getBody()) body_inventory_a.draw();
    if (body_inventory_b.getBody()) body_inventory_b.draw();
    item_swapper.draw();
}

void gui::Dual_inventory::activateEvent()
{
    inventory_a.activate();
    inventory_b.activate();
    body_inventory_a.activate();
    body_inventory_b.activate();
    item_swapper.activate();
}

void gui::Dual_inventory::deactivateEvent()
{
    inventory_a.deactivate();
    inventory_b.deactivate();
    body_inventory_a.activate();
    body_inventory_b.activate();
    item_swapper.deactivate();
}

void gui::Dual_inventory::updateInventorySizes()
{
    auto updatePane = [&](Inventory& inv, Body_inventory& binv)
    {
        if (inv.inventory() && binv.getBody())
        {
            inv.setSizeInfo({ .percentage = {0.5,0.5} });
            binv.setSizeInfo({ .percentage = {0.5,0.5} });
        }
        else if (inv.inventory())
        {
            inv.setSizeInfo({ .percentage = {0.5,1} });
            binv.setSizeInfo({ .percentage = {0.5,0} });
        }
        else
        {
            inv.setSizeInfo({ .percentage = {0.5,0} });
            binv.setSizeInfo({ .percentage = {0.5,1} });
        }
    };

    updatePane(inventory_a, body_inventory_a);
    updatePane(inventory_b, body_inventory_b);
}

gui::Dual_inventory::Dual_inventory(World& world, entt::entity entity_a, entt::entity entity_b)
  : inventory_a(world, entity_a),
    inventory_b(world, entity_b),
    item_swapper(world, {std::ref(inventory_a), std::ref(inventory_b)}),
    body_inventory_a(world, entity_a, item_swapper),
    body_inventory_b(world, entity_b, item_swapper)
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });
    bg.setAppearance({ rect({41, 32, 22, 224}) });

    body_inventory_a.setParent(this);
    body_inventory_b.setParent(this);
    body_inventory_b.setPositionInfo({ .relative_to = {1,0} });

    inventory_a.setParent(this);
    inventory_a.setAnchor(&body_inventory_a);
    inventory_a.setAnchorPositionInfo({ .side = Anchor_position_info::BOTTOM });

    inventory_b.setParent(this);
    inventory_b.setAnchor(&body_inventory_b);
    inventory_b.setAnchorPositionInfo({ .side = Anchor_position_info::BOTTOM });

    item_swapper.setSizeInfo({ .percentage = {1,1} });
}

void gui::Dual_inventory::update()
{
    updateInventorySizes();

    inventory_a.update();
    inventory_b.update();
    body_inventory_a.update();
    body_inventory_b.update();
    item_swapper.update();
}
