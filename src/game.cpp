#include "level_gen/level_structure_generator.h"
#include "level_gen/level_structure_decorator.h"
#include "level/level_structure.h"
#include "asset_storage/tile_sprite_storage.h"

#include <SFML/Graphics.hpp>

#include <iostream>

#include "gfx/animation.h"

int main()
{
    gen_params g_params;
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.min_door_count = 2;
    g_params.max_door_count = 5;

    
    std::cout << sizeof(animation) << "\n";

    vec2i l_size = {100,100};

    level_structure l_s;
    l_s.setSize(l_size);

    level_structure_generator l_gen(l_s, g_params);
    l_gen.generate();

    tile_sprite_storage::loadSprites();

    level_structure_decorator l_dec(l_s);
    l_dec.decorate();


    std::shared_ptr<std::vector<sf::Sprite>> frames = std::make_shared<std::vector<sf::Sprite>>();

    sf::Texture tex;
    tex.loadFromFile("wild_mage_frames.png");
    for (int i = 0; i < 16; i++)
    {
        frames->push_back(sf::Sprite(tex, sf::IntRect(i * 64, 0, 64, 64)));
    }

    animation anim(frames, 14);
    anim.setScale({ 4.0f, 4.0f });


    sf::RenderWindow window(sf::VideoMode(1600, 900), "");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(75);

    auto ws = [&]() -> vec2i
    {
        return { (int)window.getSize().x, (int)window.getSize().y };
    };

    const int tile_s = 64;
    vec2i top_left = l_s.getSize() / 2 * tile_s - ws()/2;

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
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5))
        {
            l_gen.generate();
            l_dec.decorate();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            top_left.y-=10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            top_left.y+=10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            top_left.x-=10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            top_left.x+=10;
        }

 
        
        vec2i first_t = top_left / tile_s;

        auto spos = [&](const vec2i pos)
        {
            vec2i shift =
            {
                top_left.x % tile_s,
                top_left.y % tile_s
            };
            return (pos - first_t) * tile_s - shift;
        };
        

        window.clear();

        for (int x = std::max(first_t.x, 0); x < l_s.getSize().x && spos({ x,0 }).x <= ws().x; x++)
        {
            for (int y = std::max(first_t.y, 0); y < l_s.getSize().y && spos({0,y}).y <= ws().y; y++)
            {
                for (auto& i : l_s.at({ x,y }).sprites)
                {
                    if (i.sprite != nullptr)
                    {
                        i.sprite->setPosition(sf::Vector2f(spos({ x,y }).x, spos({ x,y }).y));
                        window.draw(*i.sprite);
                    }
                }
            }
        }

        anim.updateFrameIdx();
        window.draw(anim);
        window.display();
    }
    l_s.printToFile("mapa.txt");
}