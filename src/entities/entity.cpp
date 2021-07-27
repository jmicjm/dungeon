#include "entity.h"
#include <algorithm>
#include <cmath>

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

std::vector<sf::Vector2i> Entity::getVisibleTiles() const
{
    auto isVisible = [&](const sf::Vector2i& dest_tile, const sf::Vector2i& dest)
    {
        const sf::Vector2i src_tile = getPosition();
        const sf::Vector2i src = src_tile*64 + sf::Vector2i{ 32, 32 };

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
            const int y_dst = (curr_tile.y * 64 + (tile_move.y > 0) * 64) - (src.y + y_dst_sum);

            const float x = src.x + vec.x * std::abs((y_dst_sum + y_dst) / static_cast<float>(vec.y));

            if (x > curr_tile.x * 64 && x < (curr_tile.x + 1) * 64)
            {
                curr_tile.y += tile_move.y;
                y_dst_sum += y_dst;

            }
            else if (x < curr_tile.x * 64 || x >(curr_tile.x + 1) * 64)
            {
                curr_tile.x += tile_move.x;
            }
            else
            {
                curr_tile += tile_move;
                y_dst_sum += y_dst;
            }
            if (y_dst_sum == vec.y) { return true; }
            if (  !m_level->ls.isPositionValid({ curr_tile.x, curr_tile.y })
                || m_level->ls.at({ curr_tile.x, curr_tile.y }).type == TILE_TYPE::WALL)
            {
                return false;
            }
        }
        return true;
    };

    auto isTileVisible = [&](const sf::Vector2i& pos)
    {
        return isVisible(pos,  pos*64 + sf::Vector2i{ 0,  0  } )
            || isVisible(pos,  pos*64 + sf::Vector2i{ 0,  64 } )
            || isVisible(pos,  pos*64 + sf::Vector2i{ 64,  0 } )
            || isVisible(pos,  pos*64 + sf::Vector2i{ 64, 64 } );
    };

    std::vector<sf::Vector2i> visible_tiles;

    for (int x = (int)getPosition().x - (int)m_vision_radius; x <= (int)getPosition().x + (int)m_vision_radius; x++)
    {
        for (int y = (int)getPosition().y - (int)m_vision_radius; y <= (int)getPosition().y + (int)m_vision_radius; y++)
        {
            if (std::sqrt(std::pow(getPosition().x-x,2)+std::pow(getPosition().y-y,2)) <= m_vision_radius)
            {
                if (isTileVisible({ x,y })) visible_tiles.push_back({ x,y });
            }
        }
    }

    return visible_tiles;
}
