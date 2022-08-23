#include "inventory.h"
#include "../../components/render_component.h"


void gui::Inventory::redraw()
{
    const float slot_size = item_field_size - item_field_border*2;
    item_field.setSizeInfo({ { slot_size, slot_size } });

    const unsigned int first_row = scroll.getTopPosition() / item_field_size;
    const unsigned int first_slot = first_row * slotsPerRow();
    for (auto slot = first_slot; slot < inventory_component.slotCount(); slot++)
    {
        const auto coords = slotToCoords(slot);
        if (coords.y > size().y) break;

        item_field.setPositionInfo({ coords + sf::Vector2f(item_field_border, item_field_border) });
        item_field.draw();
    }  

    [&]{
        for (auto it = inventory_component.usedSlots().lower_bound({ first_slot, first_slot + 1 });
             it != inventory_component.usedSlots().end();
             it++)
        {
            for (auto slot = it->lower() >= first_slot ? it->lower() : first_slot;
                 slot < it->upper();
                 slot++)
            {
                const auto coords = slotToCoords(slot);
                if (coords.y > size().y) return;

                if (auto rc = registry.try_get<Render_component>(inventory_component.get(slot)))
                {
                    sf::RenderStates st;
                    st.transform.translate(coords);
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
    }();

    if (scroll.isNeeded()) scroll.draw();
}

void gui::Inventory::activateEvent()
{
    scroll.activate();
}

void gui::Inventory::deactivateEvent()
{
    scroll.deactivate();
}

void gui::Inventory::resizeEvent(sf::Vector2f size_diff)
{
    scroll.setContentLength(std::ceil(static_cast<float>(inventory_component.slotCount()) / slotsPerRow()) * item_field_size);
}

int gui::Inventory::slotsPerRow() const
{
    auto width = scroll.isNeeded() ? size().x - scroll.size().x : size().x;
    return std::max(1.f, width / item_field_size);
}

sf::Vector2f gui::Inventory::slotToCoords(unsigned int slot) const
{
    const int row = slot / slotsPerRow();
    const int column = slot % slotsPerRow();

    return sf::Vector2f(column * item_field_size, row * item_field_size - scroll.getTopPosition());
}

gui::Inventory::Inventory(entt::registry& registry, ::Inventory& inventory_component) 
    : inventory_component(inventory_component), registry(registry)
{
    item_field.setParent(this);
    scroll.setParent(this);
    scroll.setSizeInfo({ {16,0}, {0,1} });
    scroll.setPositionInfo({ {0,0}, {0,0}, {1,0} });
}

void gui::Inventory::update()
{
    scroll.update();
}
