#include "global/window.h"
#include "world/world.h"
#include "level/level.h"
#include "entities/player.h"
#include "gfx/view_follower.h"
#include "gfx/animated_sprite/animated_sprite.h"
#include "asset_storage/texture_bank.h"
#include "input/input.h"
#include "gui/hud/hud.h"



int main()
{
    window.setFramerateLimit(60);
    
    std::shared_ptr<Animated_sprite_frames> player_frames = []()
    {
        const sf::Texture* tex = Texture_bank::getTexture("assets/characters/wild_mage_frames.png");
        std::vector<sf::IntRect> rects;
        for (int i = 0; i < 16; i++)
        {
            rects.push_back(sf::IntRect(i * 64, 0, 64, 64));
        }
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    Animated_sprite player_animation(player_frames, 16);

    std::shared_ptr<Player> player = std::make_shared<Player>(nullptr, sf::Vector2i{0,0}, player_animation);


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

    World world(world_params, player);


    View_follower vf;
    vf.target_position = [&]() { return sf::Vector2f(player->getPosition()) * 64.f + sf::Vector2f(32,0); };
    vf.velocity = 300;
    vf.edge_dst = 64*3+32;

    View_follower vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.edge_dst = 32;
    window.setView(vf_instant.followCenter(window.getView()));

    gui::Hud hud;
    hud.setPlayer(player);
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
        


        view = vf.follow(view);
        view = vf_instant.follow(view);

        sf::View rounded_view = view;
        sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
        rounded_view.setCenter(tl + view.getSize() / 2.f);
        window.setView(rounded_view);//use rounded view to avoid rendering at non integer positions

        world.update(window);
        window.draw(world);

        hud.update();
        hud.draw();


        window.display();
    }   
}