#include "component_stack.h"
#include "component_stack_helper.h"
#include "../../input/input.h"

#include "SFML/Window/Mouse.hpp"

#include <algorithm>


auto gui::Component_stack::componentIt(const Gui_component* component) const -> decltype(components)::const_iterator
{
    return std::ranges::find_if(components, [=](const auto& c) { return c.first.get() == component; });
}

void gui::Component_stack::remove(decltype(components)::const_iterator it)
{
    auto top = it == --components.end();
    components.erase(it);
    if (top && components.size()) components.back().first->activate();
}

void gui::Component_stack::redraw()
{
    for (auto& c : components) c.first->draw();
}

void gui::Component_stack::activateEvent()
{
    if (components.size()) components.back().first->activate();
}

void gui::Component_stack::deactivateEvent()
{
    if (components.size()) components.back().first->deactivate();
}

void gui::Component_stack::update()
{
    for (auto& c : components) c.first->update();

    if (isActive() && !press_blocked && components.size())
    {
        auto& top_component = *components.back().first;
        if (Input::isPressedConsume(sf::Mouse::Left) && !top_component.isHovered())
        {
            switch (components.back().second.outside_click_action)
            {
            case Component_config::OUTSIDE_CLICK_ACTION::CLOSE:
                if (auto& on_close = components.back().second.on_close) on_close();
                remove(--components.end());
                break;
            case Component_config::OUTSIDE_CLICK_ACTION::DEACTIVATE:
                if (top_component.isActive()) top_component.deactivate();
                else top_component.activate();
            }
        }
    }
    press_blocked = false;
}

void gui::Component_stack::insert(std::unique_ptr<Gui_component> component, const Component_config& config)
{
    if (component)
    {
        component->setParent(this);
        if (auto helper = dynamic_cast<Component_stack_helper*>(component.get())) helper->cs = this;
        if (isActive())
        {
            if (components.size()) components.back().first->deactivate();
            component->activate();
        }
        else component->deactivate();
        components.push_back({ std::move(component), config });
        press_blocked = true;
    }
}

void gui::Component_stack::remove(const Gui_component* component)
{
    if (auto it = componentIt(component); it != components.end())
    {
        remove(it);
    }
}

void gui::Component_stack::clear()
{
    components.clear();
}

auto gui::Component_stack::size() -> decltype(components)::size_type
{
    return components.size();
}

bool gui::Component_stack::hasComponent(const Gui_component* component) const
{
    return componentIt(component) != components.end();
}
