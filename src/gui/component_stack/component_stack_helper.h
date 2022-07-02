#pragma once
#include "component_stack.h"


namespace gui
{
    class Component_stack_helper
    {
        friend class Component_stack;
        Component_stack* cs = nullptr;

    protected:
        virtual ~Component_stack_helper() = 0;
        void removeFromComponentStack();
        void insertIntoComponentStack(std::unique_ptr<Gui_component> component, const Component_stack::Component_config& config = {});
        bool isOnComponentStack() const;
    };

    inline Component_stack_helper::~Component_stack_helper() = default;
}