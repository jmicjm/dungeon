#include "entity_context_menu.h"
#include "../primitives/button/button.h"
#include "../inventory/dual_inventory.h"
#include "../../components/item.h"
#include "../../components/inventory.h"
#include "../../components/position.h"
#include "../../components/description.h"
#include "../../world/world.h"
#include "../../global/gui_component_stack.h"
#include "../../components/body/body.h"


void gui::Entity_context_menu::redraw()
{
    bg.draw();
    description.draw();
    interaction_list.draw();
}

void gui::Entity_context_menu::activateEvent()
{
    bg.activate();
    description.activate();
    interaction_list.activate();
}

void gui::Entity_context_menu::deactivateEvent()
{
    bg.deactivate();
    description.deactivate();
    interaction_list.deactivate();
}

std::vector<std::unique_ptr<gui::Gui_component>> gui::Entity_context_menu::generateListEntries(World& world, const entt::entity entity, std::function<void()> on_action, entt::entity inventory, unsigned int inventory_slot)
{
    std::vector<std::unique_ptr<Gui_component>> entries;

    const auto addButton = [&](const std::string& text, auto on_press) {
        auto btn = std::make_unique<Button>();
        btn->setText(text);
        btn->setSizeInfo({ .fixed = {0,32}, .percentage = {1,0} });
        btn->setPressFunction(on_press);
        entries.push_back(std::move(btn));
    };

    if (world.getRegistry().try_get<::Inventory>(entity))
    {
        addButton("loot", [&world, entity, on_action] {
            auto dual_inventory = std::make_unique<Dual_inventory>(world, world.getPlayer(), entity);
            dual_inventory->setSizeInfo({ .percentage = {0.5, 0.5} });
            dual_inventory->setPositionInfo({ .relative_to = {0.5, 0.5} });
            gui_component_stack.insert(std::move(dual_inventory));
            if (on_action) on_action();
        });
    }
    if (auto item = world.getRegistry().try_get<Item>(entity))
    {
        if (inventory == entt::null)
        {
            addButton("pick", [&, entity, on_action] {
                bool inserted = (world.getRegistry().get<::Inventory>(world.getPlayer()).insert(world.getRegistry(), entity));
                if (inserted && world.getRegistry().valid(entity)) world.getRegistry().remove<Position>(entity);
                if (on_action) on_action();
                if (inserted) removeFromComponentStack();
            });
        }
        else
        {
            addButton("drop", [&, inventory, entity, inventory_slot, on_action] {
                if (auto inventory_component = world.getRegistry().try_get<::Inventory>(inventory))
                {
                    inventory_component->remove(inventory_slot);
                    if (auto player_pos = world.getRegistry().try_get<Position>(world.getPlayer()))
                    {
                        world.getRegistry().emplace_or_replace<Position>(entity, player_pos->replicate(entity));
                    }
                }
                if (on_action) on_action();
                removeFromComponentStack();
            });
        }
    }

    return entries;
}

std::string gui::Entity_context_menu::generateDescriptionText(const entt::registry& registry, entt::entity entity) const
{
    std::string desc = "";

    if (auto description_component = registry.try_get<Description>(entity))
    {
        desc += description_component->name;
    }

    if (auto body_component = registry.try_get<Body>(entity))
    {
        if (!desc.empty()) desc += "\n\n";
        desc += body_component->describe("he");
    }

    return desc;
}

gui::Entity_context_menu::Entity_context_menu(World& world, const entt::entity entity, std::function<void()> on_action, entt::entity inventory, unsigned int inventory_slot)
{
    bg.setParent(this);
    bg.setSizeInfo({ .percentage = {1,1} });

    constexpr auto border_px = 4;
    const auto descriptionHeight = [&] {
        return std::min({ description.length(), 320.f, window.getSize().y * 0.3f});
    };

    description.setParent(this);
    description.setPositionInfo({ .offset = {border_px, border_px} });
    description.setSizeFunction([=](sf::Vector2f parent_size) {
        return sf::Vector2f{ parent_size.x - border_px * 2, descriptionHeight() };
    });

    description.setString(generateDescriptionText(world.getRegistry(), entity));


    interaction_list.setParent(this);
    interaction_list.setPositionInfo({ .offset = {border_px, -border_px}, .relative_to = {0,1} });
    interaction_list.setSizeFunction([=](sf::Vector2f parent_size) {
        return sf::Vector2f{ parent_size.x - border_px * 2, parent_size.y -(descriptionHeight() + border_px * 3) };
    });
    interaction_list.setEntries(generateListEntries(world, entity, on_action, inventory, inventory_slot));

    setSizeFunction([=](sf::Vector2f parent_size) {
        const auto desired_height = interaction_list.length() + descriptionHeight() + border_px * 3;
        return sf::Vector2f{ parent_size.x * 0.3f, std::min(desired_height, parent_size.y * 0.5f) };
    });
}

void gui::Entity_context_menu::update()
{
    description.update();
    interaction_list.update();
}
