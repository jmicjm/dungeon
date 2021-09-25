#include "level/level.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

#include "gfx/animated_sprite/animated_sprite.h"
#include "asset_storage/texture_bank.h"

#include "entities/player.h"
#include "gfx/view_follower.h"



int main()
{
    Gen_params g_params;
    g_params.level_size = { 500, 500 };
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 1;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.max_empty_area_size = { 10,10 };

    Level level;
    level.create({ {64,64}, {30,30}, g_params });


    sf::RenderWindow window(sf::VideoMode(1600, 900), "");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(75);

    sf::View view(sf::FloatRect(0, 0, 1600, 900));
    float zoom = 1;
    float camera_velocity = 10;

    const sf::Texture* tex = Texture_bank::getTexture("assets/characters/wild_mage_frames.png");
    std::vector<sf::IntRect> rects;
    for (int i = 0; i < 16; i++)
    {
        rects.push_back(sf::IntRect(i * 64, 0, 64, 64));
    }
    std::shared_ptr<Animated_sprite_frames> frames = std::make_shared<Animated_sprite_frames>(tex, rects);

    Animated_sprite anim(frames, 16);
    auto anim_red = anim;
    anim_red.setColor({ 255,0,0,255 });

    std::shared_ptr<Player> player = std::make_shared<Player>(&level, sf::Vector2i{ level.structure.getRoomRect(0).tl.x,  level.structure.getRoomRect(0).tl.y }, anim);

    auto pptr = level.entities.insert({player->getPosition(), std::static_pointer_cast<Entity>(player) });

    View_follower vf;
    vf.target_position = [&]() {return sf::Vector2f(player->getPosition()) * 64.f + sf::Vector2f(32,0); };
    vf.velocity = 300;
    vf.view = &view;
    vf.edge_dst = 64*3+32;
    View_follower vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.edge_dst = 32;

    vf_instant.followCenter();


    std::chrono::steady_clock::time_point lt = std::chrono::steady_clock::now();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                view.setSize(event.size.width, event.size.height);
                break;
            case sf::Event::MouseWheelScrolled:
                zoom *= event.mouseWheelScroll.delta > 0 ? 2 : 0.5;
                view.zoom(event.mouseWheelScroll.delta > 0 ? 2 : 0.5);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            view.move(0, -camera_velocity*zoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            view.move(0, camera_velocity*zoom);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            view.move(-camera_velocity*zoom, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            view.move(camera_velocity*zoom, 0);
        }


        window.clear();
        

        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        bool move = (t - lt) >= std::chrono::milliseconds(200);


        sf::Vector2i pold = player->getPosition();
        if(move) player->updateState(true);
        if (move) lt = t;
        if (pold != player->getPosition())
        {
            level.entities.erase(pptr);
            pptr = level.entities.insert({ player->getPosition(), std::static_pointer_cast<Entity>(player) });
        }
        level.door_controller.update(window.getView());

        vf.follow();
        vf_instant.follow();

        sf::View display_view = view;
        sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
        display_view.setCenter(tl+ view.getSize()/2.f);
        window.setView(display_view);
        
        
        auto v_tiles = player->getVisibleTiles();
        std::vector<std::pair<sf::Vector2i, Tile_visibility_info>> vt;
        for (auto& i : v_tiles)
        {
            vt.push_back({i.first, i.second});
        }

        level.reveal_mask.reveal(vt);
        level.view_range_overlay.update(level, vt, level.reveal_mask, window);

        window.draw(level);


        window.display();
    }
    level.structure.printToFile("mapa.txt");
}