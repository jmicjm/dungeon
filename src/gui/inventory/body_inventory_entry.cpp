#include "body_inventory_entry.h"

void gui::Body_inventory_entry::redraw()
{
    description.draw();
    inventory.draw();
}

void gui::Body_inventory_entry::activateEvent()
{
    description.activate();
    inventory.activate();
}

void gui::Body_inventory_entry::deactivateEvent()
{
    description.deactivate();
    inventory.deactivate();
}

gui::Body_inventory_entry::Body_inventory_entry(World& world, entt::entity entity, const std::string& desc, Item_swapper& item_swapper)
    : inventory{ world, entity }
{
    inventory.setParent(this);
    inventory.setSizeInfo({ .percentage = {0.5,1} });
    description.setParent(this);
    description.setSizeInfo({ .percentage = {0.5,1} });
    description.setPositionInfo({ .relative_to = {1,0} });
    description.setString(desc);

    item_swapper.addInventory(inventory);
}

void gui::Body_inventory_entry::update()
{
    description.update();
    inventory.update();
}
