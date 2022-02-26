#include "level/level.h"
#include "entities/player.h"
#include "gfx/view_follower.h"
#include "gfx/animated_sprite/animated_sprite.h"
#include "asset_storage/texture_bank.h"
#include "input/input.h"

#include <SFML/Graphics.hpp>

#include <algorithm>


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "");
    window.setFramerateLimit(60);
    sf::View view = window.getView();


    Gen_params g_params;
    g_params.level_size = { 500, 500 };
    g_params.room_size = { { 2,2 }, { 10,10 } };
    g_params.hallway_segment_length = { 1,5 };
    g_params.hallway_segment_count = { 1,5 };
    g_params.max_empty_area_size = { 10,10 };

    Level level({ {30,30}, g_params });
    level.getStructure().printToFile("map.txt");

    
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

    std::shared_ptr<Player> player = std::make_shared<Player>(&level, level.getStructure().getRoomRect(0).tl, player_animation);
    auto pptr = level.entities.insert({ player->getPosition(), std::static_pointer_cast<Entity>(player) });

    View_follower vf;
    vf.target_position = [&]() { return sf::Vector2f(player->getPosition()) * 64.f + sf::Vector2f(32,0); };
    vf.velocity = 300;
    vf.view = &view;
    vf.edge_dst = 64*3+32;

    View_follower vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.edge_dst = 32;
    vf_instant.followCenter();



    std::chrono::steady_clock::time_point player_update_time = std::chrono::steady_clock::now();
    while (window.isOpen())
    {
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
        

        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        bool player_action_allowed = (t - player_update_time) >= std::chrono::milliseconds(200);

        if (player_action_allowed)
        {
            if(player->performAction()) player_update_time = t;
        }

        vf.follow();
        vf_instant.follow();

        sf::View rounded_view = view;
        sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
        rounded_view.setCenter(tl + view.getSize() / 2.f);
        window.setView(rounded_view);//use rounded view to avoid rendering at non integer positions

        level.updateVisibleTiles(player->getVisibleTiles(), window);
        window.draw(level);

        window.display();
    }   
}