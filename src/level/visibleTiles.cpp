#include "visibleTiles.h"
#include "../asset_storage/tile_sprite_storage.h"
#include "../utils/sf_vector2_utils.h"
#include "../components/nonpassable.h"

#include <queue>


std::unordered_map<sf::Vector2i, Tile_visibility_info> visibleTiles(const sf::Vector2i& position, const Level& level)
{
    const sf::Vector2i ts = Tile_sprite_storage::tile_size;
    auto vision_radius = 6;

    auto isVisible = [&](const sf::Vector2i& dest_point)
    {
        sf::Vector2i tile = position;
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
            if (level.isOpaque(tile) && tile != position) return false;

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
    const sf::Vector2i vision_box_tl = vecMul(position - sf::Vector2i{ static_cast<int>(vision_radius), static_cast<int>(vision_radius) }, ts);
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

    const sf::Vector2i e_pos = vecMul(position, ts);
    markAsVisited(e_pos);

    std::queue<sf::Vector2i> to_visit({ e_pos });
    while (!to_visit.empty())
    {
        const sf::Vector2i pos = to_visit.front();
        to_visit.pop();
        if (isVisible(pos))
        {
            visible_tiles[vecDiv(pos, ts)].combine({ 1,0,0,0 });
            visible_tiles[vecDiv(pos - sf::Vector2i{ ts.x, 0 }, ts)].combine({ 0,1,0,0 });
            visible_tiles[vecDiv(pos - sf::Vector2i{ 0, ts.y }, ts)].combine({ 0,0,1,0 });
            visible_tiles[vecDiv(pos - ts, ts)].combine({ 0,0,0,1 });

            const sf::Vector2i t = pos + sf::Vector2i{ 0, -ts.y };
            const sf::Vector2i b = pos + sf::Vector2i{ 0, ts.y };
            const sf::Vector2i l = pos + sf::Vector2i{ -ts.x, 0 };
            const sf::Vector2i r = pos + sf::Vector2i{ ts.x, 0 };
            const sf::Vector2i tl = pos + sf::Vector2i{ -ts.x, -ts.y };
            const sf::Vector2i tr = pos + sf::Vector2i{ ts.x, -ts.y };
            const sf::Vector2i bl = pos + sf::Vector2i{ -ts.x, ts.y };
            const sf::Vector2i br = pos + sf::Vector2i{ ts.x, ts.y };

            auto add = [&](const sf::Vector2i& pos)
            {
                if (isVisited(pos)) return;
                to_visit.push(pos);
                markAsVisited(pos);
            };

            add(t);
            add(b);
            add(l);
            add(r);
            add(tl);
            add(tr);
            add(bl);
            add(br);
        }
    }

    return visible_tiles;
}