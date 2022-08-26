#include "item_swapper.h"
#include "../../input/input.h"
#include "../../components/render_component.h"


void gui::Item_swapper::redraw()
{
    if (hold_item != entt::null)
    {
        if (auto rc = registry.try_get<Render_component>(hold_item))
        {
            sf::RenderStates st;
            st.transform.translate(sf::Vector2f{ sf::Mouse::getPosition(window) } - hold_offset);
            for (auto& [zlevel, animations] : rc->zlevel_animation_map)
            {
                for (auto& animation : animations)
                {
                    animation.updateFrameIdx();
                    draw(animation, st);
                }
            }
        }
    }
}

void gui::Item_swapper::deactivateEvent()
{
    restore();
}

void gui::Item_swapper::restore()
{
    if (hold_item != entt::null)
    {
        src_inventory->inventory()->insert(registry, hold_item, src_slot);
        hold_item = entt::null;
        src_inventory = nullptr;
    }
}

gui::Item_swapper::Item_swapper(entt::registry& registry, std::vector<std::reference_wrapper<Inventory>> inventories)
    : registry(registry), inventories(std::move(inventories)) {}

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

                if (hold_item == entt::null && Input::isPressed(sf::Mouse::Left))
                {
                    if (valid)
                    {
                        if (auto selected_item = inventory.inventory()->remove(slot_idx); selected_item != entt::null)
                        {
                            hold_item = selected_item;
                            src_slot = slot_idx;
                            src_inventory = &inventory;
                            hold_offset = offset;
                        }
                    }
                }
                else if (hold_item != entt::null && Input::isReleased(sf::Mouse::Left))
                {
                    if (!valid || !inventory.inventory()->isSlotEmpty(slot_idx))
                    {
                        restore();
                    }
                    else
                    {
                        inventory.inventory()->insert(registry, hold_item, slot_idx);
                        hold_item = entt::null;
                        src_inventory = nullptr;
                    }
                }
            }
        }
        if (hold_item != entt::null && Input::isReleased(sf::Mouse::Left))
        {
            restore();
        }
    }
}
