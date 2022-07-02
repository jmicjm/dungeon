#include "component_stack_helper.h"


void gui::Component_stack_helper::removeFromComponentStack()
{
    if (cs)
    {
        if (auto this_gc = dynamic_cast<Gui_component*>(this)) cs->remove(this_gc);
        cs = nullptr;
    }
}

void gui::Component_stack_helper::insertIntoComponentStack(std::unique_ptr<Gui_component> component, const Component_stack::Component_config& config)
{
    if (cs) cs->insert(std::move(component), config);
}

bool gui::Component_stack_helper::isOnComponentStack() const
{
    return cs;
}
