#include "player_inventory.h"
#include "../../world/world.h"
#include "../primitives/apperance_common.h"


void gui::Player_inventory::redraw()
{
    bg.draw();
    inventory.draw();
    item_swapper.draw();
}

void gui::Player_inventory::activateEvent()
{
    inventory.activate();
    item_swapper.activate();
}

void gui::Player_inventory::deactivateEvent()
{
    inventory.deactivate();
    item_swapper.deactivate();
}

gui::Player_inventory::Player_inventory(World& world, entt::entity player)
  : world(world), player(player),
    inventory(world, player),
    item_swapper(world, { std::ref(inventory) })
{
    bg.setParent(this);
    bg.setSizeInfo({ {0,0}, {1,1} });
    bg.setAppearance({ rect({41, 32, 22, 224}) });

    inventory.setParent(this);
    inventory.setSizeInfo({ {0,0}, {1,1} });

    item_swapper.setSizeInfo({ .percentage = {1,1} });
}

void gui::Player_inventory::update()
{
    inventory.update();
    item_swapper.update();
}
