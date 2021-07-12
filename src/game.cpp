#include "level_gen/level_structure_generator.h"
#include "level_gen/level_structure_decorator.h"
#include "level/level.h"
#include "asset_storage/tile_sprite_storage.h"

#include <SFML/Graphics.hpp>

#include <iostream>

#include "gfx/animated_sprite.h"
#include "asset_storage/texture_bank.h"
#include "gfx/level_tile_map.h"
#include "entities/player.h"
#include "gfx/view_follower.h"

int main()
{
    gen_params g_params;
    g_params.level_size = { 50, 50 };
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.max_empty_area_size = { 10,10 };

    level lvl;

    level_structure_generator l_gen;

    auto b = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1; i++)
    {
        l_gen.generate(lvl.ls, g_params);
    }
    auto e = std::chrono::high_resolution_clock::now();
    std::cout << "t: " << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms\n";

    tile_sprite_storage::loadSprites();

    level_structure_decorator l_dec;
    l_dec.decorate(lvl.ls);


    level_tile_map tmap;
    sf::Vector2i chunk_size = {30,30};
    tmap.populate(lvl.ls, { 64,64 }, chunk_size);


    sf::RenderWindow window(sf::VideoMode(1600, 900), "");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(75);

    sf::View view(sf::FloatRect(0, 0, 1600, 900));
    float zoom = 1;
    float camera_velocity = 100;

    const sf::Texture* tex = texture_bank::getTexture("wild_mage_frames.png");
    std::vector<sf::IntRect> rects;
    for (int i = 0; i < 16; i++)
    {
        rects.push_back(sf::IntRect(i * 64, 0, 64, 64));
    }
    std::shared_ptr<animated_sprite_frames> frames = std::make_shared<animated_sprite_frames>(tex, rects);

    animated_sprite anim(frames, 14);

    player p(&lvl, { lvl.ls.getRoomRect(0).tl.x,  lvl.ls.getRoomRect(0).tl.y }, anim);

    view_follower vf;
    vf.target_position = [&]() {return sf::Vector2f(p.getPosition()) * 64.f + sf::Vector2f(32,32); };
    vf.velocity = 300;
    vf.resetTime();
    vf.view = &view;
    vf.edge_dst = 64*3+32;


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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5))
        {
            l_gen.generate(lvl.ls, g_params);
            l_dec.decorate(lvl.ls);
            tmap.populate(lvl.ls, { 64,64 }, {10,10});
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        {
            view.rotate(-10);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            view.rotate(10);
        }

        vf.follow();

        view.setCenter(round(view.getCenter().x), round(view.getCenter().y));
        window.setView(view);
        

        window.clear();
        window.draw(tmap);

        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        bool move = (t - lt) >= std::chrono::milliseconds(200);
        p.updateState(move);
        window.draw(p);
        if (move) lt = t;

        window.display();
    }
    lvl.ls.printToFile("mapa.txt");
}