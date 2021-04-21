#include "level_structure_generator.h"
#include "../utils/rand.h"

#include <functional>


void level_structure_generator::setLevelStructure(level_structure& l)
{
    ls = &l;
}

void level_structure_generator::setGenParams(gen_params p)
{
    params = p;
}

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

void level_structure_generator::generateHallway(const vec2i start_p)
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
    auto randomDir = [&](const DIRECTION dir, const DIRECTION forbidden_dir, const vec2i pos)
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

        if (   !(pos.x <= 1                 && pos.y <= 1                )
            && !(pos.x <= 1                 && pos.y >= ls->getSize().y-2)
            && !(pos.x >= ls->getSize().x-2 && pos.y <= 1                )
            && !(pos.x >= ls->getSize().x-2 && pos.y >= ls->getSize().y-2))//ignore forbidden_dir in corners
        {
            remDir(forbidden_dir);
        }
        if (pos.x >= ls->getSize().x - 2) { remDir(DIRECTION::RIGHT); }
        if (pos.x <= 1) { remDir(DIRECTION::LEFT); }
        if (pos.y >= ls->getSize().y - 2) { remDir(DIRECTION::DOWN); }
        if (pos.y <= 1) { remDir(DIRECTION::UP); }

        return dirs[rand(0, dirs.size() - 1)];
    };
    auto initDir = [&]()
    {
        auto tileCheck = [&](const vec2i pos, std::function<bool(const TILE_TYPE)> cmp)
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
    const DIRECTION forbidden_dir = oppositeDir(dir);

    if (   adjacentTileCount(start_p, AXIS, TILE_TYPE::DOORWAY) > 0
        || sideTileCount(start_p, dir, TILE_TYPE::HALLWAY) > 0
        || start_p.x <= 1
        || start_p.y <= 1
        || start_p.x >= ls->getSize().x-2
        || start_p.y >= ls->getSize().y-2)
    {
        return;
    }

    vec2i curr_pos = start_p;
    vec2i prev_pos = start_p;
    const unsigned int m_segment_count = 
        params.max_hallway_segment_count >= params.min_hallway_segment_count
        ? rand(params.min_hallway_segment_count, params.max_hallway_segment_count)-1
        : 0;

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
                    
                    if(   (adjacentTileCount(curr_pos, AXIS, TILE_TYPE::HALLWAY) > (total_len > 1) )
                       || (total_len >= 1 && adjacentTileCount(curr_pos, AXIS, TILE_TYPE::ROOM) > 0)
                       || (adjacentTileCount(curr_pos, AXIS, TILE_TYPE::DOORWAY) > !(total_len > 1)))
                    {
                        return;
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
                else return;
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
            }
        }
        dir = randomDir(dir, forbidden_dir, curr_pos);
    }
}

bool level_structure_generator::generateRoom(const vec2i start_p)
{
    if(adjacentTileCount(start_p, AXIS|DIAG, TILE_TYPE::ROOM) > 0)
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

    while (r.size().x < max_size.x || r.size().y < max_size.y)
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
            
        if (   (!(left_expansion_possible || right_expansion_possible)  || r.size().x >= max_size.x)
            && (!(top_expansion_possible  || bottom_expansion_possible) || r.size().y >= max_size.y))//further expansion impossible
        {
            break; 
        }
    }

    if (r.size().x >= params.min_room_size.x && r.size().y >= params.min_room_size.y)
    {
        setTiles(r, TILE_TYPE::ROOM);
        ls->room_rects.push_back(r);

        const unsigned int max_door_c = rand(params.min_door_count, params.max_door_count);      
        std::vector<vec2i> possible_door_pos;

        for (int x = 0; x < r.size().x; x++)
        {
           possible_door_pos.push_back(r.tl + vec2i{ x,-1 });
           possible_door_pos.push_back(r.tl + vec2i{ x,r.size().y });
        }
        for (int y = 0; y < r.size().y; y++)
        {
            possible_door_pos.push_back(r.tl + vec2i{ -1,y });
            possible_door_pos.push_back(r.tl + vec2i{ r.size().x,y });
        }
        for (int i = possible_door_pos.size()-1; i > 0; i--)
        {
            if (ls->isPositionValid(possible_door_pos[i]))
            {
                if (ls->at(possible_door_pos[i]).type == TILE_TYPE::HALLWAY)
                {
                    possible_door_pos.erase(possible_door_pos.begin() + i);
                }
            }
            else { possible_door_pos.erase(possible_door_pos.begin() + i); }
        }

        std::vector<vec2i> door_pos;
        for (int i = 0; i < max_door_c && possible_door_pos.size()>0; i++)
        {
            int r_pos = rand(0, possible_door_pos.size() - 1);
            door_pos.push_back(possible_door_pos[r_pos]);
            possible_door_pos.erase(possible_door_pos.begin() + r_pos);
        }

        for (int i = 0; i < door_pos.size(); i++)
        {
            generateHallway(door_pos[i]);
        }
    }
    else return false;

    return true;
}

void level_structure_generator::generate()
{
    if (ls != nullptr)
    {
        fill(tile());
        ls->room_rects.clear();

        generateRoom(ls->getSize() / 2);
    }
}

unsigned int level_structure_generator::tileCount(const rect_i r)
{
    unsigned int c = 0;
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (ls->isPositionValid({ x,y })) { c++; }
        }
    }
    return c;
}

unsigned int level_structure_generator::tileCount(const rect_i r, const TILE_TYPE ttype)
{
    unsigned int c = 0;
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (ls->isPositionValid({ x,y }) && ls->at({x,y}).type == ttype) { c++; }
        }
    }
    return c;
}

unsigned int level_structure_generator::adjacentTileCount(const vec2i pos, const uint8_t area, const TILE_TYPE ttype)
{
    auto tile_check = [&](const vec2i p) 
    {
        return ls->isPositionValid(p) && ls->at(p).type == ttype; 
    };

    unsigned int c = 0;

    if (area & AXIS)
    {
        c += tile_check(pos + vec2i{ -1, 0 })
           + tile_check(pos + vec2i{  1, 0 })
           + tile_check(pos + vec2i{  0,-1 })
           + tile_check(pos + vec2i{  0, 1 });
    }
    if (area & DIAG)
    {
        c += tile_check(pos + vec2i{ -1,-1 })
           + tile_check(pos + vec2i{  1,-1 })
           + tile_check(pos + vec2i{  1, 1 })
           + tile_check(pos + vec2i{ -1, 1 });
    }

    return c;
}