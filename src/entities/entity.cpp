#include "entity.h"
#include "../utils/sf_vector2_utils.h"
#include "../asset_storage/tile_sprite_storage.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>


void Entity::updateQtPosition(const sf::Vector2i& new_pos)
{
    auto qt_ptr = [&]//todo: replace with member variable 
    {
        auto entities = level->entities.find(position);
        auto it = std::find_if(entities.begin(), entities.end(), [&](const auto& v) { return v->second.get() == this; });
        return it != entities.end() ? *it : nullptr;
    }();
    if (qt_ptr)
    {
        auto sptr = std::move(qt_ptr->second);
        level->entities.erase(qt_ptr);
        level->entities.insert({ new_pos, std::move(sptr) });
    }
}

Entity::Entity(Level* level, const sf::Vector2i& position) : level(level)
{
    setPosition(position);
}

void Entity::setPosition(const sf::Vector2i& position)
{
    //todo: if position is on unreachable tile(wall, closed door, occupied tile) find closest reachable tile
    updateQtPosition(position);
    this->position = position;
}

sf::Vector2i Entity::getPosition() const
{
    return position;
}

void Entity::move(sf::Vector2i& offset)
{
    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_pos = position + offset;
    const sf::Vector2i old_pos = position;

    if (new_pos == old_pos) return;

    const bool move_allowed_structure = [&]
    {
        const sf::Vector2i new_pos_x = position + sf::Vector2i{ offset.x, 0 };
        const sf::Vector2i new_pos_y = position + sf::Vector2i{ 0, offset.y };

        return !(level->getStructure().at(new_pos_x).type == TILE_TYPE::WALL && level->getStructure().at(new_pos_y).type == TILE_TYPE::WALL)
            && level->getStructure().at(new_pos).type != TILE_TYPE::WALL;
    }();

    if (move_allowed_structure)
    {
        const bool move_allowed_door = [&]
        {
            if (level->getStructure().at(new_pos).type == TILE_TYPE::DOORWAY) return level->door_controller.openDoor(new_pos);
            return true;
        }();

        if (move_allowed_door)
        {   
            setPosition(new_pos);
            if (level->getStructure().at(old_pos).type == TILE_TYPE::DOORWAY) level->door_controller.closeDoor(old_pos);        
        }
    }
}

std::unordered_map<sf::Vector2i, Tile_visibility_info> Entity::getVisibleTiles() const
{
    const sf::Vector2i ts = Tile_sprite_storage::tile_size;

    auto isOpaque = [&](const sf::Vector2i& pos)
    {
        auto isClosedDoor = [&]()
        {
            auto doors = level->door_controller.getDoors().find(pos);
            return doors.size() > 0 && (*doors.begin())->second.state == Door::CLOSED;
        };
        return !level->getStructure().isPositionValid(pos)
            ||  level->getStructure().at(pos).type == TILE_TYPE::WALL
            || (level->getStructure().at(pos).type == TILE_TYPE::DOORWAY && isClosedDoor());
    };

    auto isVisible = [&](const sf::Vector2i& dest_point)
    {
        sf::Vector2i tile = getPosition();
        const sf::Vector2i src = vecMul(tile, ts) + ts / 2;

        const sf::Vector2i vec = dest_point - src;
        if (std::sqrt((vec.x) * (vec.x) + (vec.y) * (vec.y)) > vision_radius * (ts.x + ts.y) / 2) return false;

        const sf::Vector2i tile_move =
        {
            vec.x / std::abs(vec.x),
            vec.y / std::abs(vec.y)
        };

        int y_dst_sum = 0;
        while (y_dst_sum != vec.y)
        {
            if (isOpaque(tile)) return false;

            const int y_dst = (tile.y * ts.y + (tile_move.y > 0) * ts.y) - (src.y + y_dst_sum);
            const int x = src.x + vec.x * (y_dst_sum + y_dst) / vec.y;

            if (x > tile.x * ts.x && x < (tile.x + 1) * ts.x)
            {
                tile.y += tile_move.y;
                y_dst_sum += y_dst;

            }
            else if (x < tile.x * ts.x || x >(tile.x + 1) * ts.x)
            {
                tile.x += tile_move.x;
            }
            else
            {
                tile += tile_move;
                y_dst_sum += y_dst;
            }
        }
        return true;
    };
    
    const int vision_box_width = vision_radius * 2 + 1;
    const sf::Vector2i vision_box_tl = vecMul(getPosition() - sf::Vector2i{ static_cast<int>(vision_radius), static_cast<int>(vision_radius) }, ts);
    std::vector<bool> visited(vision_box_width * vision_box_width, false);

    auto posToVisitedIdx = [&](const sf::Vector2i& pos)
    {
        const sf::Vector2i p = vecDiv(pos - vision_box_tl, ts);
        return p.y * vision_box_width + p.x;
    };
    auto isVisited = [&](const sf::Vector2i& pos) -> bool
    {
        const int idx = posToVisitedIdx(pos);
        if (idx < 0 || idx >= visited.size()) return true;
        return visited[idx];
    };
    auto markAsVisited = [&](const sf::Vector2i& pos)
    {
        const int idx = posToVisitedIdx(pos);
        if (idx >= 0 && idx < visited.size()) visited[idx] = true;
    };

    std::unordered_map<sf::Vector2i, Tile_visibility_info> visible_tiles;

    const sf::Vector2i e_pos = vecMul(getPosition(), ts);
    markAsVisited(e_pos);

    std::queue<sf::Vector2i> to_visit({ e_pos });
    while (!to_visit.empty())
    {
        const sf::Vector2i pos = to_visit.front();
        to_visit.pop();
        if (isVisible(pos))
        {
            visible_tiles[vecDiv(pos, ts)                          ].combine({ 1,0,0,0 });
            visible_tiles[vecDiv(pos - sf::Vector2i{ ts.x, 0 }, ts)].combine({ 0,1,0,0 });
            visible_tiles[vecDiv(pos - sf::Vector2i{ 0, ts.y }, ts)].combine({ 0,0,1,0 });
            visible_tiles[vecDiv(pos - ts, ts)                     ].combine({ 0,0,0,1 });

            const sf::Vector2i top    = pos + sf::Vector2i{  0,   -ts.y };
            const sf::Vector2i bottom = pos + sf::Vector2i{  0,    ts.y };
            const sf::Vector2i left   = pos + sf::Vector2i{ -ts.x, 0    };
            const sf::Vector2i right  = pos + sf::Vector2i{  ts.x, 0    };

            auto add = [&](const sf::Vector2i& pos)
            {
                to_visit.push(pos);
                markAsVisited(pos);
            };

            if (!isVisited(top)   ) add(top   );
            if (!isVisited(bottom)) add(bottom);
            if (!isVisited(left)  ) add(left  );
            if (!isVisited(right) ) add(right );
        }
    }

    return visible_tiles;
}