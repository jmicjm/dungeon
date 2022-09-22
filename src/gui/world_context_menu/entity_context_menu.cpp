#include "entity_context_menu.h"
#include "../primitives/button/button.h"
#include "../inventory/dual_inventory.h"
#include "../../components/item.h"
#include "../../components/inventory.h"
#include "../../components/position.h"
#include "../../world/world.h"
#include "../../global/gui_component_stack.h"


void gui::Entity_context_menu::redraw()
{
    bg.draw();
    interaction_list.draw();
}

void gui::Entity_context_menu::activateEvent()
{
    bg.activate();
    interaction_list.activate();
}

void gui::Entity_context_menu::deactivateEvent()
{
    bg.deactivate();
    interaction_list.deactivate();
}

std::vector<std::unique_ptr<gui::Gui_component>> gui::Entity_context_menu::generateListEntries(World& world, const entt::entity entity, std::function<void()> on_action)
{
    std::vector<std::unique_ptr<Gui_component>> entries;

    if (world.getRegistry().try_get<::Inventory>(entity))
    {
        auto btn = std::make_unique<Button>();
        btn->setText("loot");
        btn->setSizeInfo({ .fixed = {0,32}, .percentage = {1,0} });
        btn->setPressFunction([&world, entity, on_action] {
            auto dual_inventory = std::make_unique<Dual_inventory>(world.getRegistry(), world.getPlayer(), entity);
            dual_inventory->setSizeInfo({ .percentage = {0.5, 0.5} });
            dual_inventory->setPositionInfo({ .relative_to = {0.5, 0.5} });
            gui_component_stack.insert(std::move(dual_inventory));
            if (on_action) on_action();
        });
        entries.push_back(std::move(btn));
    }
    if (auto item = world.getRegistry().try_get<Item>(entity))
    {
        auto btn = std::make_unique<Button>();
        btn->setText("pick");
        btn->setSizeInfo({ .fixed = {0,32}, .percentage = {1,0} });
        btn->setPressFunction([&, entity, on_action] {
            bool inserted = (world.getRegistry().get<::Inventory>(world.getPlayer()).insert(world.getRegistry(), entity));
            if(inserted) world.getRegistry().remove<Position>(entity);
            if (on_action) on_action();
            if(inserted) removeFromComponentStack();
        });
        entries.push_back(std::move(btn));
    }

    return entries;
}

gui::Entity_context_menu::Entity_context_menu(World& world, const entt::entity entity, std::function<void()> on_action)
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });
    interaction_list.setParent(this);
    interaction_list.setSizeInfo({ .fixed = {-8,-8}, .percentage = {1,1} });
    interaction_list.setPositionInfo({ .offset = {4,4} });
    interaction_list.setEntries(generateListEntries(world, entity, on_action));

    setSizeFunction([&](sf::Vector2f parent_size) {
        const auto desired_height = interaction_list.length() + 8;
        return sf::Vector2f{ parent_size.x * 0.3f, std::min(desired_height, parent_size.y * 0.5f) };
    });
}

void gui::Entity_context_menu::update()
{
    interaction_list.update();
}
