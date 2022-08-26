#include "inventory.h"
#include "../../components/render_component.h"


void gui::Inventory::redraw()
{
    if (!inventory()) return;

    const float slot_size = item_field_size - item_field_border*2;
    item_field.setSizeInfo({ { slot_size, slot_size } });

    const unsigned int first_row = scroll.getTopPosition() / item_field_size;
    const unsigned int first_slot = first_row * slotsPerRow();
    for (auto slot = first_slot; slot < inventory()->slotCount(); slot++)
    {
        const auto coords = slotToCoords(slot);
        if (coords.y > size().y) break;

        item_field.setPositionInfo({ coords + sf::Vector2f(item_field_border, item_field_border) });
        item_field.draw();
    }  

    [&]{
        for (auto it = inventory()->usedSlots().lower_bound({ first_slot, first_slot + 1 });
             it != inventory()->usedSlots().end();
             it++)
        {
            for (auto slot = it->lower() >= first_slot ? it->lower() : first_slot;
                 slot < it->upper();
                 slot++)
            {
                const auto coords = slotToCoords(slot);
                if (coords.y > size().y) return;

                if (auto rc = registry.try_get<Render_component>(inventory()->get(slot)))
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
    if (!inventory()) return;
    scroll.setContentLength(std::ceil(static_cast<float>(inventory()->slotCount()) / slotsPerRow(false)) * item_field_size);
    if (scroll.isNeeded())
    {
        scroll.setContentLength(std::ceil(static_cast<float>(inventory()->slotCount()) / slotsPerRow(true)) * item_field_size);
    }
}

int gui::Inventory::slotsPerRow(bool with_scroll) const
{
    auto width = (with_scroll && scroll.isNeeded()) ? size().x - scroll.size().x : size().x;
    return std::max(1.f, width / item_field_size);
}

sf::Vector2f gui::Inventory::slotToCoords(unsigned int slot) const
{
    const int row = slot / slotsPerRow();
    const int column = slot % slotsPerRow();

    return sf::Vector2f(column * item_field_size, row * item_field_size - scroll.getTopPosition());
}

const Inventory* gui::Inventory::inventory() const
{
    return registry.valid(entity) ? registry.try_get<::Inventory>(entity) : nullptr;
}

Inventory* gui::Inventory::inventory()
{
    return const_cast<::Inventory*>(std::as_const(*this).inventory());
}

std::tuple<bool, unsigned int, sf::Vector2f> gui::Inventory::coordsToSlot(const sf::Vector2f& coords) const
{
    if (!visibleArea().contains(coords) || !inventory()) return { 0, -1u, {0,0} };

    const auto px = coords - globalPosition();
    const int x_slot = px.x / item_field_size;
    if (x_slot >= slotsPerRow()) return { 0, -1u, {0,0} };
    const int y_slot = (px.y + scroll.getTopPosition()) / item_field_size;    
    const unsigned int slot = y_slot * slotsPerRow() + x_slot;
    const sf::Vector2f offset = px - sf::Vector2f(x_slot, y_slot) * static_cast<float>(item_field_size) + sf::Vector2f(0, scroll.getTopPosition());

    return { slot < inventory()->slotCount(), slot, offset };
}

gui::Inventory::Inventory(entt::registry& registry, entt::entity entity)
  : registry(registry),
    entity(entity)
{
    item_field.setParent(this);
    scroll.setParent(this);
    scroll.setSizeInfo({ {16,0}, {0,1} });
    scroll.setPositionInfo({ .relative_to = {1,0} });
}

void gui::Inventory::update()
{
    scroll.update();
}
