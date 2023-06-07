#include "player_inventory.h"
#include "../../world/world.h"
#include "../primitives/apperance_common.h"


void gui::Player_inventory::redraw()
{
    bg.draw();
    inventory.draw();
    body_inventory.draw();
    item_swapper.draw();
}

void gui::Player_inventory::activateEvent()
{
    inventory.activate();
    body_inventory.activate();
    item_swapper.activate();
}

void gui::Player_inventory::deactivateEvent()
{
    inventory.deactivate();
    body_inventory.deactivate();
    item_swapper.deactivate();
}

gui::Player_inventory::Player_inventory(World& world, entt::entity player)
  : world(world), player(player),
    inventory(world, player),
    item_swapper(world, { std::ref(inventory) }),
    body_inventory(world, player, item_swapper)
{
    bg.setParent(this);
    bg.setSizeInfo({ {0,0}, {1,1} });
    bg.setAppearance({ rect({41, 32, 22, 224}) });

    inventory.setParent(this);
    inventory.setSizeInfo({ {0,0}, {1,0.5} });
    inventory.setPositionInfo({ .relative_to = {0, 1} });

    body_inventory.setParent(this);
    body_inventory.setSizeInfo({ {0,0}, {0.5,0.5} });

    item_swapper.setSizeInfo({ .percentage = {1,1} });
}

void gui::Player_inventory::update()
{
    inventory.update();
    body_inventory.update();
    item_swapper.update();
}
