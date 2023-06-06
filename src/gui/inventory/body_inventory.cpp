#include "body_inventory.h"
#include "body_inventory_entry.h"
#include "../../world/world.h"


void gui::Body_inventory::redraw()
{
    list.draw();
}

void gui::Body_inventory::activateEvent()
{
    list.activate();
}

void gui::Body_inventory::deactivateEvent()
{
    list.deactivate();
}

void gui::Body_inventory::fillList(Item_swapper& item_swapper)
{
    std::vector<std::unique_ptr<Gui_component>> entries;

    auto parts = bodyPartsWithInventory();

    for (auto part : parts)
    {
        entries.push_back(entryFromBodyPart(*part));
    }

    list.setEntries(std::move(entries));
}

std::vector<Body_part*> gui::Body_inventory::bodyPartsWithInventory()
{
    std::vector<Body_part*> bparts;
    
    if (auto body = getBody(); body && body->graph.root)
    {
        body->graph.foreach([&](Body_part& bp) {
            if (bp.getInventory(world.getRegistry())) bparts.push_back(&bp);
        });
    }

    return bparts;
}

std::unique_ptr<gui::Gui_component> gui::Body_inventory::entryFromBodyPart(const Body_part& bp)
{
    std::unique_ptr<Gui_component> comp = std::make_unique<Body_inventory_entry>(world, bp.inventory_entity, bp.name(), item_swapper);
    comp->setSizeInfo({ .fixed = {0,64}, .percentage = {1,0} });
    return comp;
}

Body* gui::Body_inventory::getBody()
{
    return world.getRegistry().valid(entity) ? world.getRegistry().try_get<Body>(entity) : nullptr;
}

gui::Body_inventory::Body_inventory(World& world, entt::entity entity, Item_swapper& item_swapper)
    : world{ world }, entity{ entity }, item_swapper{ item_swapper }
{
    list.setParent(this);
    list.setSizeInfo({ .percentage = {1,1} });
    fillList(item_swapper);
}

void gui::Body_inventory::update()
{
    list.update();
}
