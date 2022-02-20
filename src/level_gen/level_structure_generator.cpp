#include "level_structure_generator.h"
#include "../utils/rand.h"

#include <algorithm>
#include <random>


void Level_structure_generator::fill(const Tile tile)
{
    for (int x = 0; x < ls->getSize().x; x++)
    {
        for (int y = 0; y < ls->getSize().y; y++)
        {
            ls->at({ x,y }) = tile;
        }
    }
}

void Level_structure_generator::setTiles(Rect_i r, const TILE_TYPE ttype)
{
    clipRectToLevelSize(r);
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            ls->at({ x,y }).type = ttype;
        }
    }
}

bool Level_structure_generator::generateHallway(const sf::Vector2i& start_p)
{
    enum class DIRECTION
    {
        LEFT, RIGHT, UP, DOWN
    };
    auto advance = [](sf::Vector2i pos, const DIRECTION dir)
    {
        switch (dir)
        {
        case DIRECTION::LEFT:
            return pos + sf::Vector2i{ -1, 0 };
        case DIRECTION::UP:
            return pos + sf::Vector2i{  0,-1 };
        case DIRECTION::RIGHT:
            return pos + sf::Vector2i{  1, 0 };
        case DIRECTION::DOWN:
            return pos + sf::Vector2i{  0, 1 };
        }
    };
    auto oppositeDir = [](const DIRECTION dir)
    {
        switch (dir)
        {
        case DIRECTION::LEFT:
            return DIRECTION::RIGHT;
        case DIRECTION::UP:
            return DIRECTION::DOWN;
        case DIRECTION::RIGHT:
            return DIRECTION::LEFT;
        case DIRECTION::DOWN:
            return DIRECTION::UP;
        }
    };
    auto randomDir = [&](const DIRECTION dir, const sf::Vector2i& pos)
    {
        std::vector<DIRECTION> dirs = { DIRECTION::LEFT, DIRECTION::UP, DIRECTION::RIGHT, DIRECTION::DOWN };
        auto remDir = [&](const DIRECTION dir)
        {
            if (dirs.size() > 1)
            {
                for (int i = 0; i < dirs.size(); i++)
                {
                    if (dirs[i] == dir) { dirs.erase(dirs.begin() + i); break; }
                }
            }
        };
        remDir(oppositeDir(dir));

        if (pos.x >= ls->getSize().x - 2) { remDir(DIRECTION::RIGHT); }
        if (pos.x <= 1)                   { remDir(DIRECTION::LEFT);  }
        if (pos.y >= ls->getSize().y - 2) { remDir(DIRECTION::DOWN);  }
        if (pos.y <= 1)                   { remDir(DIRECTION::UP);    }

        return dirs[rand(0, dirs.size() - 1)];
    };
    auto initDir = [&]()
    {
        auto tileCheck = [&](const sf::Vector2i& pos, const auto& cmp)
        {
            return ls->isPositionValid(pos) && cmp(ls->at(pos).type);
        };

        if (adjacentTileCount(start_p, AXIS, TILE_TYPE::ROOM) > 0)
        {
            auto r_cmp = [](const TILE_TYPE ttype) {return ttype == TILE_TYPE::ROOM; };

            if (tileCheck(start_p + sf::Vector2i{ -1,  0 }, r_cmp)) { return DIRECTION::RIGHT; }
            if (tileCheck(start_p + sf::Vector2i{  1,  0 }, r_cmp)) { return DIRECTION::LEFT;  }
            if (tileCheck(start_p + sf::Vector2i{  0, -1 }, r_cmp)) { return DIRECTION::DOWN;  }
            if (tileCheck(start_p + sf::Vector2i{  0,  1 }, r_cmp)) { return DIRECTION::UP;    }
        }
        else
        {
            auto w_cmp = [](const TILE_TYPE ttype) {return ttype != TILE_TYPE::WALL; };

            if (tileCheck(start_p + sf::Vector2i{ -1,  0 }, w_cmp)) { return DIRECTION::RIGHT; }
            if (tileCheck(start_p + sf::Vector2i{  1,  0 }, w_cmp)) { return DIRECTION::LEFT;  }
            if (tileCheck(start_p + sf::Vector2i{  0, -1 }, w_cmp)) { return DIRECTION::DOWN;  }
            if (tileCheck(start_p + sf::Vector2i{  0,  1 }, w_cmp)) { return DIRECTION::UP;    }
        }

        return DIRECTION::UP;
    };
    auto sideTileCount = [&](const sf::Vector2i& pos, const DIRECTION dir, const TILE_TYPE ttype)
    {
        auto isT = [&](const sf::Vector2i& offset)
        {
            return ls->isPositionValid(pos + offset) && ls->at(pos + offset).type == ttype;
        };

        switch (dir)
        {
        case DIRECTION::LEFT:
        case DIRECTION::RIGHT:
            return isT({ 0,-1 }) + isT({ 0,1 });
        case DIRECTION::UP:
        case DIRECTION::DOWN:
            return isT({ -1,0 }) + isT({ 1,0 });
        }
    };
    auto isTouchingCorner = [&](const sf::Vector2i& pos, const DIRECTION dir)
    {
        auto isWall = [&](const sf::Vector2i& offset)
        {
            return ls->isPositionValid(pos + offset) && ls->at(pos + offset).type == TILE_TYPE::WALL;
        };

        static const auto tl = sf::Vector2i{ -1,-1 };
        static const auto tr = sf::Vector2i{  1,-1 };
        static const auto bl = sf::Vector2i{ -1, 1 };
        static const auto br = sf::Vector2i{  1, 1 };

        switch (dir)
        {
        case DIRECTION::LEFT:
            return !isWall(bl) || !isWall(tl);
        case DIRECTION::UP:
            return !isWall(tl) || !isWall(tr);
        case DIRECTION::RIGHT:
            return !isWall(tr) || !isWall(br);
        case DIRECTION::DOWN:
            return !isWall(br) || !isWall(bl);
        }
    };

    DIRECTION dir = initDir();//direction opposite to adjacent room/hallway
    DIRECTION prev_dir = dir;

    if (   ls->at(start_p).type != TILE_TYPE::WALL
        || adjacentTileCount(start_p, AXIS, TILE_TYPE::DOORWAY) > 0
        || sideTileCount(start_p, dir, TILE_TYPE::HALLWAY) > 0
        || start_p.x <= 1
        || start_p.y <= 1
        || start_p.x >= ls->getSize().x-2
        || start_p.y >= ls->getSize().y-2)
    {
        return false;
    }

    sf::Vector2i curr_pos = start_p;
    sf::Vector2i prev_pos = start_p;

    std::vector<sf::Vector2i> modified_tiles;
    auto undoHallway = [&]()
    {
        for (const auto& tile_pos : modified_tiles)
        {
            ls->at(tile_pos).type = TILE_TYPE::WALL;
        }
    };

    unsigned int total_len = 0;
    const unsigned int m_segment_count = rand(params.hallway_segment_count.getMin(), params.hallway_segment_count.getMax()) - 1;
    for (int seg = m_segment_count; seg >= 0; seg--)
    {
        const unsigned int m_seg_len = rand(params.hallway_segment_length.getMin(), params.hallway_segment_length.getMax());

        for (unsigned int len = 0; len < m_seg_len; len++, total_len++)
        {
            if (   !(curr_pos.x <= 1                 && dir == DIRECTION::LEFT )
                && !(curr_pos.y <= 1                 && dir == DIRECTION::UP   )
                && !(curr_pos.x >= ls->getSize().x-2 && dir == DIRECTION::RIGHT)
                && !(curr_pos.y >= ls->getSize().y-2 && dir == DIRECTION::DOWN ))
            {
                if (ls->at(curr_pos).type == TILE_TYPE::WALL)
                {
                    if (adjacentTileCount(curr_pos, AXIS, TILE_TYPE::ROOM) > 0)
                    {
                        ls->at(curr_pos).type = TILE_TYPE::DOORWAY;
                    }
                    else
                    {
                        ls->at(curr_pos).type = TILE_TYPE::HALLWAY;
                    }
                    modified_tiles.push_back(curr_pos);

                    /*
                    1. undo in cases of three-way doors
                    2. undo in cases like:
                       _CH_
                       HHHH
                       where H-hallway, C-curr_pos
                     */
                    if (ls->at(advance(curr_pos, prev_dir)).type == TILE_TYPE::DOORWAY //1
                        || sideTileCount(curr_pos, prev_dir, TILE_TYPE::WALL) != 2) //2
                    {
                        undoHallway();
                        return false;
                    }
                    
                    //room or another hallway reached
                    if(adjacentTileCount(curr_pos, AXIS, TILE_TYPE::WALL) < adjacentTileCount(curr_pos, AXIS)-1)
                    {
                       return true;
                    }

                    /*
                    undo in cases like: 
                    RR__
                    RR__
                    __CH
                    where R-room, H-hallway, C-curr_pos
                    */
                    if (isTouchingCorner(curr_pos, prev_dir))
                    {
                        undoHallway();
                        return false;
                    }

                    prev_dir = dir;
                    prev_pos = curr_pos;
                    curr_pos = advance(curr_pos, dir);
                }
                else return true;
            }
            else break;
        }

        //room placement
        if (seg == 0)
        {
            if (ls->isPositionValid(curr_pos) && ls->at(curr_pos).type == TILE_TYPE::WALL)
            {
                if (generateRoom(curr_pos))
                {
                    ls->at(prev_pos).type = TILE_TYPE::DOORWAY;
                    return true;
                }
                else seg++;
            }
        }
        dir = randomDir(dir, curr_pos);
    }
}

bool Level_structure_generator::generateRoom(const sf::Vector2i& start_p)
{
    if(    ls->at(start_p).type != TILE_TYPE::WALL
        || adjacentTileCount(start_p, AXIS|DIAG, TILE_TYPE::ROOM) > 0
        || adjacentTileCount(start_p, AXIS|DIAG, TILE_TYPE::WALL) < adjacentTileCount(start_p, AXIS|DIAG)-1)
    {
        return false;
    }

    Rect_i r = { start_p,start_p };
    const sf::Vector2i max_size = { rand(params.room_size.getMin().x, params.room_size.getMax().x),
                                    rand(params.room_size.getMin().y, params.room_size.getMax().y) };

    bool left_expansion_possible   = true;
    bool top_expansion_possible    = true;
    bool right_expansion_possible  = true;
    bool bottom_expansion_possible = true;

    while (   (left_expansion_possible || right_expansion_possible ) && r.size().x < max_size.x
           || (top_expansion_possible  || bottom_expansion_possible) && r.size().y < max_size.y)
    {
        auto expansionCheck = [&](const Rect_i& scan_area)
        {
            return !contains(scan_area, [&](const sf::Vector2i& pos) { return ls->at(pos).type != TILE_TYPE::WALL; });
        };

        left_expansion_possible = r.tl.x > 1 && expansionCheck({ r.tl + sf::Vector2i{-2,-1}, r.tl + sf::Vector2i{-1,r.size().y} });
        if (left_expansion_possible && r.size().x < max_size.x) { r.tl.x--; }

        top_expansion_possible = r.tl.y > 1 && expansionCheck({ r.tl + sf::Vector2i{-1,-2}, r.tl + sf::Vector2i{r.size().x,-1} });
        if (top_expansion_possible && r.size().y < max_size.y) { r.tl.y--; }

        right_expansion_possible = r.br.x < ls->getSize().x-2 && expansionCheck({ r.br + sf::Vector2i{1,-r.size().y}, r.br + sf::Vector2i{2,1} });
        if (right_expansion_possible && r.size().x < max_size.x) { r.br.x++; }

        bottom_expansion_possible = r.br.y < ls->getSize().y-2 && expansionCheck({ r.br + sf::Vector2i{-r.size().x, 1}, r.br + sf::Vector2i{1,2} });
        if (bottom_expansion_possible && r.size().y < max_size.y) { r.br.y++; }
    }

    if (r.size().x >= params.room_size.getMin().x && r.size().y >= params.room_size.getMin().y)
    {
        setTiles(r, TILE_TYPE::ROOM);
        ls->room_rects.push_back(r);

        return true;
    }

    return false;
}

void Level_structure_generator::fillEmptyAreas()
{
    sf::Vector2i tl = (ls->getSize() - params.max_empty_area_size) / 2;
    sf::Vector2i br = tl;

    auto fill = [&](const sf::Vector2i& p)
    {
        const Rect_i check_area = { p, p + params.max_empty_area_size };

        while (!contains(check_area, [&](const sf::Vector2i& pos) { return ls->at(pos).type != TILE_TYPE::WALL; }))
        {
            if(!fillEmptyArea(check_area)) return;
        }
    };

    const sf::Vector2i min_p = { 1, 1 };
    const sf::Vector2i max_p = ls->getSize() - params.max_empty_area_size - sf::Vector2i{ 2,2 };

    std::vector<sf::Vector2i> subareas;

    int layer = 0;
    while (tl != min_p || br != max_p)
    { 
        for (int x = tl.x; x <= br.x; x++)
        {
            subareas.push_back({ x, tl.y });
            subareas.push_back({ x, br.y });
        }
        for (int y = tl.y; y <= br.y; y++)
        {
            subareas.push_back({ tl.x, y });
            subareas.push_back({ br.x, y });
        }

        tl = { std::max(min_p.x, tl.x-1),
               std::max(min_p.y, tl.y-1) };
        br = { std::min(max_p.x, br.x+1),
               std::min(max_p.y, br.y+1) };

        if (layer++ % (params.max_empty_area_size.x + params.max_empty_area_size.y) / 2 == 0 
            || (tl == min_p && br == max_p))
        {
            static std::mt19937 rng(std::random_device{}());
            std::shuffle(subareas.begin(), subareas.end(), rng);

            for (const auto& subarea : subareas)
            {
                fill(subarea);
            }
            subareas.clear();
        }
    }
}

bool Level_structure_generator::fillEmptyArea(Rect_i area)
{
    auto tryGen = [&](const sf::Vector2i& pos)
    {
        if (adjacentTileCount(pos, AXIS, TILE_TYPE::WALL) < adjacentTileCount(pos, AXIS))
        {
            return generateHallway(pos);
        }
        return false;
    };

    const sf::Vector2i min_p = { 1, 1 };
    const sf::Vector2i max_p = ls->getSize() - sf::Vector2i{ 2,2 };

    while (area != Rect_i{ min_p, max_p })
    {
        std::vector<sf::Vector2i> to_try;
        to_try.reserve(std::max(0, 2 * area.size().x) + std::max(0, 2 * (area.size().y - 2)));

        for (int x = area.tl.x; x <= area.br.x; x++)
        {
            to_try.push_back({ x, area.tl.y });
            to_try.push_back({ x, area.br.y });
        }
        for (int y = area.tl.y+1; y <= area.br.y-1; y++)
        {
            to_try.push_back({ area.tl.x, y });
            to_try.push_back({ area.br.x, y });
        }

        static std::mt19937 rng(std::random_device{}());
        std::shuffle(to_try.begin(), to_try.end(), rng);

        for (const auto& pos : to_try)
        {
            if (tryGen(pos)) return true;
        }

        area =
        {
            { std::max(min_p.x, area.tl.x - 1),
              std::max(min_p.y, area.tl.y - 1) },
            { std::min(max_p.x, area.br.x + 1),
              std::min(max_p.y, area.br.y + 1) }
        };
    }
    return false;
}

void Level_structure_generator::generate(Level_structure& l, Gen_params p) &&
{
    ls = &l;
    params = p;

    ls->setSize(p.level_size);
    fill(Tile());
    ls->room_rects.clear();

    generateRoom(ls->getSize() / 2);

    fillEmptyAreas();
}

void Level_structure_generator::clipRectToLevelSize(Rect_i& r)
{
    r =
    {
        { std::clamp(r.tl.x, 0, ls->getSize().x-1),
          std::clamp(r.tl.y, 0, ls->getSize().y-1) },
        { std::clamp(r.br.x, 0, ls->getSize().x-1),
          std::clamp(r.br.y, 0, ls->getSize().y-1) }
    };
}

template<typename T>
unsigned int Level_structure_generator::adjacentTileCount(const sf::Vector2i& pos, const uint8_t area, const T& pred)
{
    unsigned int c = 0;

    if (area & AXIS)
    {
        c += pred(pos + sf::Vector2i{ -1, 0 })
           + pred(pos + sf::Vector2i{  1, 0 })
           + pred(pos + sf::Vector2i{  0,-1 })
           + pred(pos + sf::Vector2i{  0, 1 });
    }
    if (area & DIAG)
    {
        c += pred(pos + sf::Vector2i{ -1,-1 })
           + pred(pos + sf::Vector2i{  1,-1 })
           + pred(pos + sf::Vector2i{  1, 1 })
           + pred(pos + sf::Vector2i{ -1, 1 });
    }

    return c;
}

unsigned int Level_structure_generator::adjacentTileCount(const sf::Vector2i& pos, const uint8_t area)
{
    auto pred = [&](const sf::Vector2i& p)
    {
        return ls->isPositionValid(p);
    };

    return adjacentTileCount(pos, area, pred);
}

unsigned int Level_structure_generator::adjacentTileCount(const sf::Vector2i& pos, const uint8_t area, const TILE_TYPE ttype)
{
    auto pred = [&](const sf::Vector2i& p)
    {
        return ls->isPositionValid(p) && ls->at(p).type == ttype; 
    };

    return adjacentTileCount(pos, area, pred);
}

template<typename T>
bool Level_structure_generator::contains(Rect_i r, const T& pred)
{
    clipRectToLevelSize(r);
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (pred({ x,y })) return true;
        }
    }
    return false;
}