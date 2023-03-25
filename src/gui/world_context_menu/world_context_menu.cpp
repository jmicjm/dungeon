#include "world_context_menu.h"
#include "../../world/world.h"
#include "../../level/level.h"
#include "../../utils/sf_vector2_utils.h"
#include "../../asset_storage/tile_sprite_storage.h"
#include "../primitives/button/button.h"
#include "../../components/render_component.h"
#include "../../components/description.h"
#include "entity_context_menu.h"
#include "../../global/gui_component_stack.h"
#include <algorithm>


void gui::World_context_menu::redraw()
{
    bg.draw();
    entity_list.draw();
}

void gui::World_context_menu::activateEvent()
{
    bg.activate();
    entity_list.activate();
}

void gui::World_context_menu::deactivateEvent()
{
    bg.deactivate();
    entity_list.deactivate();
}

std::vector<std::unique_ptr<gui::Gui_component>> gui::World_context_menu::generateListEntries(World& world, const sf::Vector2f& world_px)
{
    std::vector<std::unique_ptr<Gui_component>> entries;

    auto enttc = 0;

    const auto world_tile = sf::Vector2i{ vecDiv(world_px, Tile_sprite_storage::tile_size) };

    world.getEntities().at(world.getCurrentLevel()).forEach({ world_tile - sf::Vector2i{1,1}, world_tile + sf::Vector2i{2,2} }, [&](auto value) {
        const auto [coords, entity] = value;

        auto isHovered = [&] {
            const auto tile_px = world_px - sf::Vector2f{ vecMul(coords, Tile_sprite_storage::tile_size) };

            if (const auto* rc = world.getRegistry().try_get<Render_component>(entity))
            {
                const auto& animation_map = world.getCurrentLevel()->isVisible(coords) ? rc->zlevel_animation_map : rc->shadow_zlevel_animation_map;
                for (const auto& [zlevel, animations] : animation_map)
                {
                    for (const auto& animation : animations)
                    {
                        if (animation.getGlobalBounds().contains(tile_px)) return true;
                    }
                }
            }
            return false;
        };

        if (isHovered() && entity != world.getPlayer())
        {
            auto description = world.getRegistry().try_get<Description>(entity);
            const std::string& name = description ? description->name : "<unnamed>";

            auto btn = std::make_unique<Button>();
            btn->setReleasedText(name);
            btn->setPressedHoveredText(name);
            btn->setReleasedText(name);
            btn->setReleasedHoveredText(name);
            btn->setSizeInfo({ .fixed = {0,32}, .percentage = {1,0} });
            btn->setPressFunction([&, entity, world_px] {
                auto entity_menu = std::make_unique<Entity_context_menu>(world, entity, [&, world_px] {
                    entity_list.setEntries(generateListEntries(world, world_px)); 
                });
                entity_menu->setPositionInfo({ .relative_to = {0.5,0.5} });
                gui_component_stack.insert(std::move(entity_menu));
            });

            if (!entries.empty())
            {
                auto separator = std::make_unique<Frame>();
                separator->setSizeInfo({ .fixed = {0,2}, .percentage = {1,0} });
                entries.push_back(std::move(separator));
            }

            entries.push_back(std::move(btn));
        }
        });


    return entries;
}

gui::World_context_menu::World_context_menu(World& world, const sf::Vector2f& world_px)
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });

    entity_list.setParent(this);
    entity_list.setSizeInfo({ .fixed = {-8,-8}, .percentage = {1,1} });
    entity_list.setPositionInfo({ .offset = {4,4} });
    entity_list.setEntries(generateListEntries(world, world_px));

    setSizeFunction([&](sf::Vector2f parent_size) {
        const auto desired_height = entity_list.length() + 8;
        return sf::Vector2f{ parent_size.x * 0.2f, std::min(desired_height, parent_size.y * 0.5f) };
    });
}

void gui::World_context_menu::update()
{
    entity_list.update();
}

size_t gui::World_context_menu::entryCount() const
{
    return entity_list.entryCount();
}
