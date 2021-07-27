#include "level/level.h"
#include "asset_storage/tile_sprite_storage.h"

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
    g_params.level_size = { 50, 50 };
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.max_empty_area_size = { 10,10 };

    Level level;

    Tile_sprite_storage::loadSprites();
    level.create({ {64,64}, {30,30}, g_params });


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

    Animated_sprite anim(frames, 16);

    Player player(&level, { level.ls.getRoomRect(0).tl.x,  level.ls.getRoomRect(0).tl.y }, anim);

    View_follower vf;
    vf.target_position = [&]() {return sf::Vector2f(player.getPosition()) * 64.f + sf::Vector2f(32,0); };
    vf.velocity = 300;
    vf.view = &view;
    vf.edge_dst = 64*3+32;
    View_follower vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.edge_dst = 32;


    sf::RenderTexture view_range_overlay_tex;
    std::vector<Tile_visibility_info> last_visible_tiles;
    sf::View last_display_view;

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
            level.create({ {64,64}, {30,30}, g_params });
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
        player.updateState(move);
        vf.follow();
        vf_instant.follow();

        sf::View display_view = view;
        sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
        display_view.setCenter(tl+ view.getSize()/2.f);
        window.setView(display_view);

        window.draw(level.tmap);
        window.draw(player);
        if (move) lt = t;

        
        std::vector<Tile_visibility_info> visible_tiles = player.getVisibleTiles();

        if (   visible_tiles != last_visible_tiles 
            || display_view.getTransform() != last_display_view.getTransform() 
            || display_view.getSize() != last_display_view.getSize()
           )
        {
            view_range_overlay_tex.create(window.getSize().x, window.getSize().y);
            view_range_overlay_tex.clear({ 0,0,0,224 });
            view_range_overlay_tex.setView(window.getView());

            static sf::Sprite tile_tl_tr_bl_br(*Texture_bank::getTexture("tile_tl_tr_bl_br.png"));
            tile_tl_tr_bl_br.setOrigin(8, 8);
            static sf::Sprite wall_tl_tr(*Texture_bank::getTexture("wall_tl_tr.png"));
            wall_tl_tr.setOrigin(8, 8);
            static sf::Sprite wall_bl_br(*Texture_bank::getTexture("wall_bl_br.png"));
            wall_bl_br.setOrigin(8, 8);
            static sf::Sprite wall_tl_bl(*Texture_bank::getTexture("wall_tl_bl.png"));
            wall_tl_bl.setOrigin(8, 8);
            static sf::Sprite wall_tr_br(*Texture_bank::getTexture("wall_tr_br.png"));
            wall_tr_br.setOrigin(8, 8);
            static sf::Sprite wall_tl_tr_bl(*Texture_bank::getTexture("wall_tl_tr_bl.png"));
            wall_tl_tr_bl.setOrigin(8, 8);
            static sf::Sprite wall_tl_tr_br(*Texture_bank::getTexture("wall_tl_tr_br.png"));
            wall_tl_tr_br.setOrigin(8, 8);
            static sf::Sprite wall_tl_bl_br(*Texture_bank::getTexture("wall_tl_bl_br.png"));
            wall_tl_bl_br.setOrigin(8, 8);
            static sf::Sprite wall_tr_bl_br(*Texture_bank::getTexture("wall_tr_bl_br.png"));
            wall_tr_bl_br.setOrigin(8, 8);
            static sf::Sprite wall_tl(*Texture_bank::getTexture("wall_tl.png"));
            wall_tl.setOrigin(8, 8);
            static sf::Sprite wall_tr(*Texture_bank::getTexture("wall_tr.png"));
            wall_tr.setOrigin(8, 8);
            static sf::Sprite wall_bl(*Texture_bank::getTexture("wall_bl.png"));
            wall_bl.setOrigin(8, 8);
            static sf::Sprite wall_br(*Texture_bank::getTexture("wall_br.png"));
            wall_br.setOrigin(8, 8);

            for (const auto& tile : visible_tiles)
            {
                sf::Sprite* spr = nullptr;

                if (level.ls.at({ tile.position.x, tile.position.y }).type != TILE_TYPE::WALL)
                {
                    spr = &tile_tl_tr_bl_br;
                }
                else if (tile.tl && tile.tr && tile.bl)
                {
                    spr = &wall_tl_tr_bl;
                }
                else if (tile.tl && tile.tr && tile.br)
                {
                    spr = &wall_tl_tr_br;
                }
                else if (tile.tl && tile.bl && tile.br)
                {
                    spr = &wall_tl_bl_br;
                }
                else if (tile.tr && tile.bl && tile.br)
                {
                    spr = &wall_tr_bl_br;
                }
                else if (tile.tl && tile.bl)
                {
                    spr = &wall_tl_bl;
                }
                else if (tile.tr && tile.br)
                {
                    spr = &wall_tr_br;
                }
                else if (tile.tl && tile.tr)
                {
                    spr = &wall_tl_tr;
                }
                else if (tile.bl && tile.br)
                {
                    spr = &wall_bl_br;
                }
                else if (tile.tl)
                {
                    spr = &wall_tl;
                }
                else if (tile.tr)
                {
                    spr = &wall_tr;
                }
                else if (tile.bl)
                {
                    spr = &wall_bl;
                }
                else if (tile.br)
                {
                    spr = &wall_br;
                }

                if (spr != nullptr)
                {
                    spr->setPosition(sf::Vector2f{ tile.position }*64.f);

                    view_range_overlay_tex.draw(*spr, sf::BlendMultiply);
                }

            }
            view_range_overlay_tex.display();
        }
        last_visible_tiles = visible_tiles;
        last_display_view = display_view;

 
        window.setView(sf::View{sf::FloatRect{{0.f,0.f}, sf::Vector2f{window.getSize()}}});
        window.draw(sf::Sprite{ view_range_overlay_tex.getTexture() });


        window.display();
    }
    level.ls.printToFile("mapa.txt");
}