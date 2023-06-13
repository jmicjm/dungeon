#include "player_inventory.h"
#include "../../world/world.h"
#include "../primitives/apperance_common.h"


void gui::Player_inventory::redraw()
{
    bg.draw();
    inventory.draw();
    body_inventory.draw();
    body_stats.draw();
    item_swapper.draw();
}

void gui::Player_inventory::activateEvent()
{
    inventory.activate();
    body_inventory.activate();
    body_stats.activate();
    item_swapper.activate();
}

void gui::Player_inventory::deactivateEvent()
{
    inventory.deactivate();
    body_inventory.deactivate();
    body_stats.deactivate();
    item_swapper.deactivate();
}

gui::Player_inventory::Player_inventory(World& world, entt::entity player)
  : world{ world },
    player{ player },
    inventory{ world, player },
    item_swapper{ world, { std::ref(inventory) } },
    body_inventory{ world, player, item_swapper },
    body_stats{ world.getRegistry(), player }
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });
    bg.setAppearance({ rect({41, 32, 22, 224}) });

    inventory.setParent(this);
    inventory.setSizeInfo({ .percentage = {1,0.5} });
    inventory.setPositionInfo({ .relative_to = {0, 1} });

    body_inventory.setParent(this);
    body_inventory.setSizeInfo({ .percentage = {0.5,0.5} });

    body_stats.setParent(this);
    body_stats.setSizeInfo({ .percentage = {0.5,0.5} });
    body_stats.setPositionInfo({ .relative_to = {1,0} });

    item_swapper.setSizeInfo({ .percentage = {1,1} });
}

void gui::Player_inventory::update()
{
    inventory.update();
    body_inventory.update();
    body_stats.update();
    item_swapper.update();
}
