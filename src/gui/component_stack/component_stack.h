#pragma once
#include "../gui_component.h"

#include <vector>
#include <utility>
#include <memory>
#include <functional>


namespace gui
{
    class Component_stack : public Gui_component
    {
    public:
        struct Component_config
        {
            enum class OUTSIDE_CLICK_ACTION
            {
                DEACTIVATE, CLOSE, NOTHING
            } outside_click_action = OUTSIDE_CLICK_ACTION::CLOSE;

            std::function<void()> on_close = nullptr;

            Component_config() {}
        };
    private:
        std::vector<std::pair<std::unique_ptr<Gui_component>, Component_config>> components;
        bool press_blocked = false;

        auto componentIt(const Gui_component* component) const -> decltype(components)::const_iterator;
        void remove(decltype(components)::const_iterator it);

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        void update() override;

        void insert(std::unique_ptr<Gui_component> component, const Component_config& config = {});
        void remove(const Gui_component* component);
        void clear();
        auto size() -> decltype(components)::size_type;
        bool hasComponent(const Gui_component* component) const;
    };
}