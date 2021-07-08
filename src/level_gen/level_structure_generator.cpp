#include "level_structure_generator.h"
#include "../utils/rand.h"

void level_structure_generator::fill(const tile t)
{
    for (int x = 0; x < ls->getSize().x; x++)
    {
        for (int y = 0; y < ls->getSize().y; y++)
        {
            ls->at({ x,y }) = t;
        }
    }
}

void level_structure_generator::setTiles(const rect_i r, const TILE_TYPE tile)
{
    for (int x = r.tl.x; x<= r.br.x;x++)
    {
        for (int y = r.tl.y;y<= r.br.y;y++)
        {
            if (ls->isPositionValid({ x,y }))
            {
                ls->at({ x,y }).type = tile;
            }
        }
    }
}

bool level_structure_generator::generateHallway(const vec2i start_p)
{
    enum class DIRECTION
    {
        LEFT, RIGHT, UP, DOWN
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
    auto randomDir = [&](const DIRECTION dir, const vec2i pos)
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
        if (pos.x <= 1) { remDir(DIRECTION::LEFT); }
        if (pos.y >= ls->getSize().y - 2) { remDir(DIRECTION::DOWN); }
        if (pos.y <= 1) { remDir(DIRECTION::UP); }

        return dirs[rand(0, dirs.size() - 1)];
    };
    auto initDir = [&]()
    {
        auto tileCheck = [&](const vec2i pos, const auto& cmp)
        {
            return ls->isPositionValid(pos) && cmp(ls->at(pos).type);
        };

        if (adjacentTileCount(start_p, AXIS, TILE_TYPE::ROOM) > 0)
        {
            auto r_cmp = [](const TILE_TYPE ttype) {return ttype == TILE_TYPE::ROOM; };

            if (tileCheck(start_p + vec2i{ -1,  0 }, r_cmp)) { return DIRECTION::RIGHT; }
            if (tileCheck(start_p + vec2i{  1,  0 }, r_cmp)) { return DIRECTION::LEFT;  }
            if (tileCheck(start_p + vec2i{  0, -1 }, r_cmp)) { return DIRECTION::DOWN;  }
            if (tileCheck(start_p + vec2i{  0,  1 }, r_cmp)) { return DIRECTION::UP;    }
        }
        else
        {
            auto w_cmp = [](const TILE_TYPE ttype) {return ttype != TILE_TYPE::WALL; };

            if (tileCheck(start_p + vec2i{ -1,  0 }, w_cmp)) { return DIRECTION::RIGHT; }
            if (tileCheck(start_p + vec2i{  1,  0 }, w_cmp)) { return DIRECTION::LEFT;  }
            if (tileCheck(start_p + vec2i{  0, -1 }, w_cmp)) { return DIRECTION::DOWN;  }
            if (tileCheck(start_p + vec2i{  0,  1 }, w_cmp)) { return DIRECTION::UP;    }
        }

        return DIRECTION::UP;
    };

    auto sideTileCount = [&](const vec2i pos, const DIRECTION dir, const TILE_TYPE ttype)
    {
        switch (dir)
        {
        case DIRECTION::LEFT:
        case DIRECTION::RIGHT:
        {
            const vec2i t = pos + vec2i{ 0,-1 };
            const vec2i b = pos + vec2i{ 0, 1 };
            return tileCount({ t,t }, ttype) + tileCount({ b, b }, ttype);
        }
        case DIRECTION::UP:
        case DIRECTION::DOWN:
        {
            const vec2i l = pos + vec2i{ -1,0 };
            const vec2i r = pos + vec2i{  1,0 };
            return tileCount({ l,l }, ttype) + tileCount({ r, r }, ttype);
        }
        }
    };

    DIRECTION dir = initDir();//direction opposite to adjacent room/hallway

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

    vec2i curr_pos = start_p;
    vec2i prev_pos = start_p;
    const unsigned int m_segment_count = rand(params.min_hallway_segment_count, params.max_hallway_segment_count)-1;

    unsigned int total_len = 0;
    for (int seg = m_segment_count; seg >= 0; seg--)
    {
        const unsigned int m_seg_len = rand(params.min_hallway_segment_length, params.max_hallway_segment_length);

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
                    
                    if(adjacentTileCount(curr_pos, AXIS, TILE_TYPE::WALL) < adjacentTileCount(curr_pos, AXIS)-1)
                    {
                       return true;
                    }

                    prev_pos = curr_pos;
                    switch (dir)
                    {
                    case DIRECTION::LEFT:
                        curr_pos.x--;
                        break;
                    case DIRECTION::UP:
                        curr_pos.y--;
                        break;
                    case DIRECTION::RIGHT:
                        curr_pos.x++;
                        break;
                    case DIRECTION::DOWN:
                        curr_pos.y++;
                    }
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
                ls->at(prev_pos).type = TILE_TYPE::DOORWAY;
                if (!generateRoom(curr_pos))
                {
                    if (total_len > 1)
                    {
                        ls->at(prev_pos).type = TILE_TYPE::HALLWAY;
                    }
                    seg++;
                }
                else return true;
            }
        }
        dir = randomDir(dir, curr_pos);
    }
}

bool level_structure_generator::generateRoom(const vec2i start_p)
{
    if(    ls->at(start_p).type != TILE_TYPE::WALL
        || adjacentTileCount(start_p, AXIS|DIAG, TILE_TYPE::ROOM) > 0)
    {
        return false;
    }

    rect_i r = { start_p,start_p };
    const vec2i max_size = 
    { 
        rand(params.min_room_size.x, params.max_room_size.x),
        rand(params.min_room_size.y, params.max_room_size.y) 
    };

    bool left_expansion_possible   = true;
    bool top_expansion_possible    = true;
    bool right_expansion_possible  = true;
    bool bottom_expansion_possible = true;

    while (   (left_expansion_possible || right_expansion_possible ) && r.size().x < max_size.x
           || (top_expansion_possible  || bottom_expansion_possible) && r.size().y < max_size.y)
    {
        auto expansionCheck = [&](const rect_i scan_area)
        {
            return !(tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area));
        };

        left_expansion_possible = r.tl.x > 1 && expansionCheck({ r.tl + vec2i{-2,-1}, r.tl + vec2i{-1,r.size().y} });
        if (left_expansion_possible && r.size().x < max_size.x) { r.tl.x--; }

        top_expansion_possible = r.tl.y > 1 && expansionCheck({ r.tl + vec2i{-1,-2}, r.tl + vec2i{r.size().x,-1} });
        if (top_expansion_possible && r.size().y < max_size.y) { r.tl.y--; }

        right_expansion_possible = r.br.x < ls->getSize().x-2 && expansionCheck({ r.br + vec2i{1,-r.size().y}, r.br + vec2i{2,1} });
        if (right_expansion_possible && r.size().x < max_size.x) { r.br.x++; }

        bottom_expansion_possible = r.br.y < ls->getSize().y-2 && expansionCheck({ r.br + vec2i{-r.size().x, 1}, r.br + vec2i{1,2} });
        if (bottom_expansion_possible && r.size().y < max_size.y) { r.br.y++; }
    }

    if (r.size().x >= params.min_room_size.x && r.size().y >= params.min_room_size.y)
    {
        setTiles(r, TILE_TYPE::ROOM);
        ls->room_rects.push_back(r);

        return true;
    }

    return false;
}

void level_structure_generator::fillEmptyAreas()
{
    for (int x = 1; x < ls->getSize().x - params.max_empty_area.x-1; x++)
    {
        for (int y = 1; y < ls->getSize().y - params.max_empty_area.y-1; y++)
        {
            const rect_i check_area = { vec2i{x,y}, vec2i{x,y} + params.max_empty_area };

            while (tileCount(check_area, TILE_TYPE::WALL) == tileCount(check_area))
            {
                fillEmptyArea(check_area);    
            }
        }
    }
}

void level_structure_generator::fillEmptyArea(rect_i area)
{
    auto tryGen = [&](const vec2i pos)
    {
        if (adjacentTileCount(pos, AXIS, TILE_TYPE::WALL) < adjacentTileCount(pos, AXIS))
        {
            return generateHallway(pos);
        }
        return false;
    };

    bool try_left   = true;
    bool try_top    = true;
    bool try_right  = true;
    bool try_bottom = true;

    while (area != rect_i{ {1, 1}, ls->getSize() - vec2i{2,2} })
    {
        for (int x = area.tl.x; x <= area.br.x; x++)
        {
            if (try_top    && tryGen({ x, area.tl.y })) return; 
            if (try_bottom && tryGen({ x, area.br.y })) return; 
        }
        for (int y = area.tl.y; y <= area.br.y; y++)
        {
            if (try_left   && tryGen({ area.tl.x, y })) return; 
            if (try_right  && tryGen({ area.br.x, y })) return;
        }

        const rect_i old_area = area;
        area =
        {
            {
            std::max(1, area.tl.x - 1),
            std::max(1, area.tl.y - 1)
            },
            {
            std::min(ls->getSize().x - 2, area.br.x + 1),
            std::min(ls->getSize().y - 2, area.br.y + 1)
            }
        };

        try_left   = old_area.tl.x != area.tl.x;
        try_top    = old_area.tl.y != area.tl.y;
        try_right  = old_area.br.x != area.br.x;
        try_bottom = old_area.br.y != area.br.y;
    }
}

void level_structure_generator::generate(level_structure& l, gen_params p)
{
    ls = &l;
    params = p;

    fill(tile());
    ls->room_rects.clear();

    generateRoom(ls->getSize() / 2);

    fillEmptyAreas();
}

template<typename T>
unsigned int level_structure_generator::tileCount(const rect_i r, const T& pred)
{
    unsigned int c = 0;
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (pred({x,y})) { c++; }
        }
    }
    return c;
}

unsigned int level_structure_generator::tileCount(const rect_i r)
{
    auto pred = [&](const vec2i p)
    {
        return ls->isPositionValid(p);
    };

    return tileCount(r, pred);
}

unsigned int level_structure_generator::tileCount(const rect_i r, const TILE_TYPE ttype)
{
    auto pred = [&](const vec2i p)
    {
        return ls->isPositionValid(p) && ls->at(p).type == ttype;
    };

    return tileCount(r, pred);
}

template<typename T>
unsigned int level_structure_generator::adjacentTileCount(const vec2i pos, const uint8_t area, const T& pred)
{
    unsigned int c = 0;

    if (area & AXIS)
    {
        c += pred(pos + vec2i{ -1, 0 })
           + pred(pos + vec2i{  1, 0 })
           + pred(pos + vec2i{  0,-1 })
           + pred(pos + vec2i{  0, 1 });
    }
    if (area & DIAG)
    {
        c += pred(pos + vec2i{ -1,-1 })
           + pred(pos + vec2i{  1,-1 })
           + pred(pos + vec2i{  1, 1 })
           + pred(pos + vec2i{ -1, 1 });
    }

    return c;
}

unsigned int level_structure_generator::adjacentTileCount(const vec2i pos, const uint8_t area)
{
    auto pred = [&](const vec2i p)
    {
        return ls->isPositionValid(p);
    };

    return adjacentTileCount(pos, area, pred);
}

unsigned int level_structure_generator::adjacentTileCount(const vec2i pos, const uint8_t area, const TILE_TYPE ttype)
{
    auto pred = [&](const vec2i p) 
    {
        return ls->isPositionValid(p) && ls->at(p).type == ttype; 
    };

    return adjacentTileCount(pos, area, pred);
}