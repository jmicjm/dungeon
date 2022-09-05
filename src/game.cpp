#include "global/window.h"
#include "global/gui_component_stack.h"
#include "world/world.h"
#include "level/level.h"
#include "input/input.h"
#include "gui/hud/hud.h"
#include "components/character_update_tags.h"
#include "asset_storage/tile_sprite_storage.h"
#include "utils/sf_vector2_utils.h"
#include "components/inventory.h"
#include "gui/inventory/dual_inventory.h"
#include "entities/items/createItem.h"
#include "components/stackable_item.h"
#include "components/render_component.h"


int main()
{
    window.setFramerateLimit(60);

    Level_structure_params structure_params;
    structure_params.level_size = { 500, 500 };
    structure_params.room_size = { { 2,2 }, { 10,10 } };
    structure_params.hallway_segment_length = { 1,5 };
    structure_params.hallway_segment_count = { 1,5 };
    structure_params.max_empty_area_size = { 10,10 };

    Level_structure_params small_structure_params = structure_params;
    small_structure_params.level_size = { 50,50 };

    Level_structure_params hallways_structure_params = structure_params;
    hallways_structure_params.hallway_segment_length = { 10,50 };

    World_params world_params =
    {
        { 
            { structure_params },
            { small_structure_params },
            { hallways_structure_params },
            { structure_params }
        },
    };

    World world(world_params);

    auto& inv = world.getRegistry().get<Inventory>(world.getPlayer());
    inv.insert(world.getRegistry(), items::createItem(world.getRegistry(), items::Item_id::BLOODTHIRSTY_BLADE), 4);
    inv.insert(world.getRegistry(), items::createItem(world.getRegistry(), items::Item_id::DAGGER), 5);
    inv.insert(world.getRegistry(), items::createItem(world.getRegistry(), items::Item_id::LONGSWORD), 18);
    auto coins = items::createItem(world.getRegistry(), items::Item_id::COINS);
    world.getRegistry().get<Stackable_item>(coins).setAmount(999);
    auto coins2 = world.getRegistry().get<Stackable_item>(coins).take(world.getRegistry(), 16);
    world.getRegistry().get<Stackable_item>(coins2).setAmount(999);
    auto coins3 = world.getRegistry().get<Stackable_item>(coins2).take(world.getRegistry(), 16);
    world.getRegistry().get<Stackable_item>(coins3).setAmount(4);
    inv.insert(world.getRegistry(), coins);
    inv.insert(world.getRegistry(), coins2,1);
    inv.insert(world.getRegistry(), coins3,3);


    gui::Hud hud(world.getRegistry(), world.getPlayer());
    hud.activate();


    while (window.isOpen())
    {
        auto view = window.getView();

        Input::update();
        sf::Event event;
        while (window.pollEvent(event))
        {
            Input::update(event);
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                view.setSize(event.size.width, event.size.height);
                break;
            case sf::Event::MouseWheelScrolled:
                view.zoom(event.mouseWheelScroll.delta > 0 ? 2 : 0.5);
            }
        }
        window.clear();
        window.setView(view);

        world.update(window);
        window.draw(world);

        if (world.getRegistry().all_of<Character_updating>(world.getPlayer()) && !gui_component_stack.size()) hud.activate();
        else hud.deactivate();

        hud.update();
        hud.draw();

        if (Input::isPressed(sf::Mouse::Left) && !gui_component_stack.size() && world.getCurrentLevel())
        {
            const auto world_px = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            const auto world_tile = sf::Vector2i{ vecDiv(world_px, Tile_sprite_storage::tile_size) };

            world.getEntities().at(world.getCurrentLevel()).forEachUntil({ world_tile - sf::Vector2i{1,1}, world_tile + sf::Vector2i{2,2} }, [&](auto value) {
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
                    if (auto inv = world.getRegistry().try_get<Inventory>(entity))
                    {
                        auto dual_inventory = std::make_unique<gui::Dual_inventory>(world.getRegistry(), world.getPlayer(), entity);
                        dual_inventory->setSizeInfo({ .percentage = {0.5, 0.5} });
                        dual_inventory->setPositionInfo({ .relative_to = {0.5, 0.5} });
                        gui_component_stack.insert(std::move(dual_inventory));

                        return true;
                    }
                }
                return false;
            });
        }

        gui_component_stack.update();
        gui_component_stack.draw();


        window.display();
    }   
}