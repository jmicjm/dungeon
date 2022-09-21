#pragma once
#include "../gui_component.h"
#include "../component_stack/component_stack_helper.h"
#include "../primitives/list/list.h"
#include "../primitives/frame/frame.h"
#include "../../world/world.fwd.h"
#include "SFML/System/Vector2.hpp"
#include "entt/entt.hpp"
#include <vector>
#include <memory>
#include <functional>


namespace gui
{
    class Entity_context_menu : public Gui_component, public Component_stack_helper
    {
        Frame bg;
        List interaction_list;

        void redraw() override;

        void activateEvent() override;
        void deactivateEvent() override;

        std::vector<std::unique_ptr<Gui_component>> generateListEntries(World& world, const entt::entity entity, std::function<void()> on_action);

    public:
        Entity_context_menu(World& world, const entt::entity entity, std::function<void()> on_action = nullptr);

        void update() override;
    };
}