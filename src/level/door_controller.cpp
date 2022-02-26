#include "door_controller.h"
#include "../asset_storage/texture_bank.h"
#include "../gfx/utils/visibleAreaBounds.h"
#include "level.h"

#include <algorithm>


void Door_controller::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    const auto vis = visibleAreaBoundsTiles(rt.getView());
    auto drs = doors.find({ vis.first, vis.second });

    std::sort(drs.begin(), drs.end(), [](const auto& a, const auto& b) { return a->first.y < b->first.y; });

    for (auto& i : drs)
    {
        st.texture = i->second.getSprite().texture;
        rt.draw(i->second.getSprite().vertices, 4, sf::Quads, st);
    }
}

Door_controller::Door_controller(const Level* level)
{
    this->level = level;

    const Primitive_sprite front_open  (Texture_bank::getTexture("assets/tiles/Door_front_open.png"  ), { {0,0}, {68,74 } });
    const Primitive_sprite front_closed(Texture_bank::getTexture("assets/tiles/Door_front_closed.png"), { {0,0}, {68,64 } });
    const Primitive_sprite side_open   (Texture_bank::getTexture("assets/tiles/Door_side_open.png"   ), { {0,0}, {69,100} });
    const Primitive_sprite side_closed (Texture_bank::getTexture("assets/tiles/Door_side_closed.png" ), { {0,0}, {64,100} });

    const auto& ls = level->getStructure();
    doors = Quadtree<Door>({ {0,0}, { ls.getSize().x, ls.getSize().y } });

    for (int x = 1; x < ls.getSize().x - 1; x++)
    {
        for (int y = 1; y < ls.getSize().y - 1; y++)
        {
            if (ls.at({ x,y }).type == TILE_TYPE::DOORWAY)
            {
                if (ls.at({ x - 1,y }).type == TILE_TYPE::WALL && ls.at({ x + 1,y }).type == TILE_TYPE::WALL)
                {
                    auto open = front_open;
                    open.move(sf::Vector2f(x * 64 - 2, y * 64 - 20));
                    auto closed = front_closed;
                    closed.move(sf::Vector2f(x * 64 - 2, y * 64 - 10));

                    doors.insert({ {x,y}, Door{open, closed} });
                }
                else if (ls.at({ x,y - 1 }).type == TILE_TYPE::WALL && ls.at({ x,y + 1 }).type == TILE_TYPE::WALL)
                {
                    auto open = side_open;
                    open.move(sf::Vector2f(x * 64 + 1, y * 64 - 34));
                    auto closed = side_closed;
                    closed.move(sf::Vector2f(x * 64 + 1, y * 64 - 34));

                    doors.insert({ {x,y}, Door{open, closed} });
                }
            }
        }
    }
}

const Quadtree<Door>& Door_controller::getDoors() const
{
    return doors;
}

bool Door_controller::openDoor(const sf::Vector2i& pos)
{
    auto doors = this->doors.find(pos);
    if (doors.size() > 0)
    {
        doors[0]->second.state = Door::OPEN;
        return true;
    }
    return false;
}

bool Door_controller::closeDoor(const sf::Vector2i& pos)
{
    auto doors = this->doors.find(pos);
    if (doors.size() > 0 && level->entities.find(pos).size() == 0)
    {
        doors[0]->second.state = Door::CLOSED;
        return true;
    }
    return false;
}
