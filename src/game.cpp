#include "global/window.h"
#include "global/gui_component_stack.h"
#include "world/world.h"
#include "level/level.h"
#include "input/input.h"
#include "gui/hud/hud.h"


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


    gui::Hud hud;
    hud.setPlayer(world.getRegistry(), world.getPlayer());
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

        hud.update();
        hud.draw();

        gui_component_stack.update();
        gui_component_stack.draw();


        window.display();
    }   
}