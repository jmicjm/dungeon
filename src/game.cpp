#include "level_gen/level_structure_generator.h"
#include "level_gen/level_structure_decorator.h"
#include "level/level.h"
#include "asset_storage/tile_sprite_storage.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>

#include "gfx/animated_sprite/animated_sprite.h"
#include "asset_storage/texture_bank.h"
#include "gfx/level_tile_map/level_tile_map.h"
#include "entities/player.h"
#include "gfx/view_follower.h"

int main()
{
    Gen_params g_params;
    g_params.level_size = { 500, 500 };
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.max_empty_area_size = { 10,10 };

    Level lvl;
    Level_tile_map tmap;

    Tile_sprite_storage::loadSprites();

    auto gen = [&]()
    {
        auto bg = std::chrono::high_resolution_clock::now();
        Level_structure_generator{}.generate(lvl.ls, g_params);
        auto eg = std::chrono::high_resolution_clock::now();
        std::cout << "tgen: " << std::chrono::duration_cast<std::chrono::milliseconds>(eg - bg).count() << " ms\n";

        auto bd = std::chrono::high_resolution_clock::now();
        Level_structure_decorator{}.decorate(lvl.ls);
        auto ed = std::chrono::high_resolution_clock::now();
        std::cout << "tdec: " << std::chrono::duration_cast<std::chrono::milliseconds>(ed - bd).count() << " ms\n";

        sf::Vector2i chunk_size = { 30,30 };

        auto bp = std::chrono::high_resolution_clock::now();
        tmap.populate(lvl.ls, { 64,64 }, chunk_size);
        auto ep = std::chrono::high_resolution_clock::now();
        std::cout << "tpop: " << std::chrono::duration_cast<std::chrono::milliseconds>(ep - bp).count() << " ms\n";

        std::cout << "ttotal: " << std::chrono::duration_cast<std::chrono::milliseconds>(ep - bg).count() << " ms\n";
    };
    gen();

    


    sf::RenderWindow window(sf::VideoMode(1600, 901), "");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(75);

    sf::View view(sf::FloatRect(0, 0, 1600, 901));
    float zoom = 1;
    float camera_velocity = 10;

    const sf::Texture* tex = Texture_bank::getTexture("wild_mage_frames.png");
    std::vector<sf::IntRect> rects;
    for (int i = 0; i < 16; i++)
    {
        rects.push_back(sf::IntRect(i * 64, 0, 64, 64));
    }
    std::shared_ptr<Animated_sprite_frames> frames = std::make_shared<Animated_sprite_frames>(tex, rects);

    Animated_sprite anim(frames, 14);

    Player player(&lvl, { lvl.ls.getRoomRect(0).tl.x,  lvl.ls.getRoomRect(0).tl.y }, anim);

    View_follower vf;
    vf.target_position = [&]() {return sf::Vector2f(player.getPosition()) * 64.f + sf::Vector2f(32,0); };
    vf.velocity = 0;// 300;
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
            gen();
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

        

        
        

        window.clear();
        

        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        bool move = (t - lt) >= std::chrono::milliseconds(200);
        player.updateState(move);
        vf.follow();

        sf::View display_view = view;
        sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
        display_view.setCenter(tl+ view.getSize()/2.f);
        window.setView(display_view);

        window.draw(tmap);
        window.draw(player);
        if (move) lt = t;

        

        window.display();
    }
    lvl.ls.printToFile("mapa.txt");
}