#pragma once
#include "../gui_component.h"
#include "../primitives/list/list.h"
#include "../primitives/frame/frame.h"
#include "../../world/world.fwd.h"
#include "SFML/System/Vector2.hpp"
#include <vector>
#include <memory>


namespace gui
{
    class World_context_menu : public Gui_component
    {
        Frame bg;
        List entity_list;


        void redraw() override;

        void activateEvent() override;
        void deactivateEvent() override;

        std::vector<std::unique_ptr<Gui_component>> generateListEntries(World& world, const sf::Vector2f& world_px);

    public:
        World_context_menu(World& world, const sf::Vector2f& world_pixel);

        void update() override;
    };
}