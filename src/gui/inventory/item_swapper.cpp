#include "item_swapper.h"
#include "../../input/input.h"
#include "../../components/stackable_item.h"
#include "common/drawItem.h"
#include "../../world/world.h"
#include "../world_context_menu/entity_context_menu.h"
#include "../../global/gui_component_stack.h"
#include "../../global/window.h"


void gui::Item_swapper::redraw()
{
    if (isHolding())
    {
        sf::RenderStates st;
        st.transform.translate(sf::Vector2f{ sf::Mouse::getPosition(window) } - hold_offset);
        drawItem(window, st, world.getRegistry(), hold_item);
    }
}

void gui::Item_swapper::deactivateEvent()
{
    restore();
}

void gui::Item_swapper::restore()
{
    if (isHolding())
    {
        src_inventory->inventory()->insert(world.getRegistry(), hold_item, src_slot);
        hold_item = entt::null;
        src_inventory = nullptr;
    }
}

bool gui::Item_swapper::isHolding() const
{
    return world.getRegistry().valid(hold_item);
}

gui::Item_swapper::Item_swapper(World& world, std::vector<std::reference_wrapper<Inventory>> inventories)
    : world(world), inventories(std::move(inventories)) {}

gui::Item_swapper::~Item_swapper()
{
    restore();
}

void gui::Item_swapper::update()
{
    if (isActive())
    {
        for (Inventory& inventory : inventories)
        {
            const auto mouse_coords = sf::Vector2f{ sf::Mouse::getPosition(window) };
            if (inventory.visibleArea().contains(mouse_coords))
            {
                const auto [valid, slot_idx, offset] = inventory.coordsToSlot(mouse_coords);

                if (!valid) continue;

                else if (!isHolding() && Input::isPressedConsume(sf::Mouse::Left))
                {
                    if (auto selected_item = inventory.inventory()->remove(slot_idx); world.getRegistry().valid(selected_item))
                    {
                        hold_item = selected_item;
                        src_slot = slot_idx;
                        src_inventory = &inventory;
                        hold_offset = offset;
                    }
                }
                else if (isHolding() && Input::isReleasedConsume(sf::Mouse::Left))
                {
                    if (inventory.inventory()->insert(world.getRegistry(), hold_item, slot_idx))
                    {            
                        if (slot_idx == src_slot && &inventory == src_inventory)
                        {
                            auto context_menu = std::make_unique<gui::Entity_context_menu>(world, inventory.inventory()->get(slot_idx), [] {}, inventory.inventoryEntity(), slot_idx);
                            context_menu->setPositionInfo({ .relative_to = {0.5,0.5} });

                            gui_component_stack.insert(std::move(context_menu));
                        }

                        hold_item = entt::null;
                        src_inventory = nullptr;
                    }
                    else if (auto hovered_item = inventory.inventory()->get(slot_idx); world.getRegistry().valid(hovered_item))
                    {
                        inventory.inventory()->remove(slot_idx);
                        inventory.inventory()->insert(world.getRegistry(), hold_item, slot_idx);
                        src_inventory->inventory()->insert(world.getRegistry(), hovered_item, src_slot);
                        hold_item = entt::null;
                        src_inventory = nullptr;
                    }
                }
            }
        }
        if (isHolding() && Input::isReleasedConsume(sf::Mouse::Left))
        {
            restore();
        }
    }
}

void gui::Item_swapper::addInventory(Inventory& inventory)
{
    inventories.push_back(inventory);
}
