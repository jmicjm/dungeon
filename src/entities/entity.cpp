#include "entity.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <type_traits>


Entity::Entity(Level* level, const sf::Vector2i& position) : m_level(level)
{
    setPosition(position);
}

void Entity::setPosition(const sf::Vector2i& position)
{
    //todo: if position is on unreachable tile(wall, closed door, occupied tile) find closest reachable tile
    m_position = position;
}

sf::Vector2i Entity::getPosition() const
{
    return m_position;
}

void Entity::move(sf::Vector2i& offset)
{
    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_pos = m_position + offset;

    const sf::Vector2i new_pos_x = m_position + sf::Vector2i{ offset.x, 0 };
    const sf::Vector2i new_pos_y = m_position + sf::Vector2i{ 0, offset.y };

    if (!( m_level->ls.at({ new_pos_x.x, new_pos_x.y }).type == TILE_TYPE::WALL && m_level->ls.at({ new_pos_y.x, new_pos_y.y }).type == TILE_TYPE::WALL)
        && m_level->ls.at({ new_pos.x, new_pos.y }).type != TILE_TYPE::WALL)
    {
        m_position = new_pos;
    }
}

void Entity::updateState(const bool make_action) {}

std::vector<std::pair<sf::Vector2i, Tile_visibility_info>> Entity::getVisibleTiles() const
{
    const sf::Vector2i ts = m_level->tile_size;

    auto vecMul = [](const auto& veca, const auto& vecb) -> sf::Vector2<typename std::common_type<decltype(veca.x), decltype(vecb.x)>::type>
    {
        return { veca.x * vecb.x, veca.y * vecb.y };
    };
    auto isOpaque = [&](const sf::Vector2i& pos)
    {
        auto isClosedDoor = [&]()
        {
            auto doors = m_level->doors.doors.find(pos);
            return doors.size() > 0 && (*doors.begin())->second.state == Door::CLOSED;
        };

        return !m_level->ls.isPositionValid({ pos.x, pos.y })
            || m_level->ls.at({ pos.x, pos.y }).type == TILE_TYPE::WALL
            || isClosedDoor();        
    };

    auto isVisible = [&](const sf::Vector2i& dest_tile, const sf::Vector2i& dest)
    {
        const sf::Vector2i src_tile = getPosition();
        const sf::Vector2i src = vecMul(src_tile, ts) + ts / 2;

        const sf::Vector2i vec = dest - src;
        const sf::Vector2i tile_move =
        {
            static_cast<int>(vec.x / std::abs(vec.x)),
            static_cast<int>(vec.y / std::abs(vec.y))
        };

        int y_dst_sum = 0;
        sf::Vector2i curr_tile = src_tile;
        while (curr_tile != dest_tile)
        {
            const int y_dst = (curr_tile.y * ts.y + (tile_move.y > 0) * ts.y) - (src.y + y_dst_sum);

            const float x = src.x + vec.x * std::abs((y_dst_sum + y_dst) / static_cast<float>(vec.y));

            if (x > curr_tile.x * ts.x && x < (curr_tile.x + 1) * ts.x)
            {
                curr_tile.y += tile_move.y;
                y_dst_sum += y_dst;

            }
            else if (x < curr_tile.x * ts.x || x >(curr_tile.x + 1) * ts.x)
            {
                curr_tile.x += tile_move.x;
            }
            else
            {
                curr_tile += tile_move;
                y_dst_sum += y_dst;
            }
            if (y_dst_sum == vec.y) { return true; }
            if (isOpaque(curr_tile))
            {
                return false;
            }
        }
        return true;
    };

    auto getTileVisibilityInfo = [&](const sf::Vector2i& pos) -> Tile_visibility_info
    {
        auto isInRange = [&](const sf::Vector2i& a, const sf::Vector2i& b)
        {
            return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) <= m_vision_radius*(ts.x+ts.y)/2;
        };
        const sf::Vector2i tl = vecMul(pos, ts);
        const sf::Vector2i tr = vecMul(pos, ts) + sf::Vector2i{ ts.x,  0  };
        const sf::Vector2i bl = vecMul(pos, ts) + sf::Vector2i{ 0,   ts.y };
        const sf::Vector2i br = vecMul(pos, ts) + ts;

        const sf::Vector2i e_center = vecMul(getPosition(), ts) + ts/2;

        return { isInRange(e_center, tl) && isVisible(pos,  tl),
                 isInRange(e_center, tr) && isVisible(pos,  tr),
                 isInRange(e_center, bl) && isVisible(pos,  bl),
                 isInRange(e_center, br) && isVisible(pos,  br) };
    };

    std::vector<std::pair<sf::Vector2i, Tile_visibility_info>> visible_tiles;

    const sf::Vector2i e_pos = getPosition();
    std::queue<sf::Vector2i> to_visit({ e_pos });

    const int vision_box_width = m_vision_radius * 2 + 1;
    const sf::Vector2i vision_box_tl = e_pos - sf::Vector2i{ static_cast<int>(m_vision_radius), static_cast<int>(m_vision_radius) };

    std::vector<bool> visited(vision_box_width * vision_box_width, false);
    
    auto posToVisitedIdx = [&](const sf::Vector2i& pos)
    {
        const sf::Vector2i p = pos - vision_box_tl;

        return p.y * vision_box_width + p.x;
    };

    auto markAsVisited = [&](const sf::Vector2i& pos)
    {
        const int idx = posToVisitedIdx(pos);
        if (idx >= 0 && idx < visited.size()) visited[idx] = true;    
    };

    markAsVisited(e_pos);

    auto isVisited = [&](const sf::Vector2i& pos) -> bool
    {
        const int idx = posToVisitedIdx(pos);
        if (idx < 0 || idx >= visited.size()) return true;
        return visited[idx];
    };

    while (!to_visit.empty())
    {
        const sf::Vector2i pos = to_visit.front();
        to_visit.pop();
        const Tile_visibility_info tvi = getTileVisibilityInfo(pos);
        if (tvi.isVisible())
        {
            visible_tiles.push_back({ pos,tvi });

            const sf::Vector2i top    = pos + sf::Vector2i{  0,-1 };
            const sf::Vector2i bottom = pos + sf::Vector2i{  0, 1 };
            const sf::Vector2i left   = pos + sf::Vector2i{ -1, 0 };
            const sf::Vector2i right  = pos + sf::Vector2i{  1, 0 };

            auto add = [&](const sf::Vector2i& pos)
            {
                to_visit.push(pos);
                markAsVisited(pos);
            };
            if ((tvi.tl || tvi.tr) && !isVisited(top)   ) add(top   );
            if ((tvi.bl || tvi.br) && !isVisited(bottom)) add(bottom);
            if ((tvi.tl || tvi.bl) && !isVisited(left)  ) add(left  );
            if ((tvi.tr || tvi.br) && !isVisited(right) ) add(right );
        }
    }

    return visible_tiles;
}