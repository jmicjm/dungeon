#pragma once
#include "vec2.h"
#include "rect.h"
#include "gen_params.h"

#include <vector>
#include <string>

enum class TILE_TYPE
{
    WALL, ROOM, HALLWAY
};



class map
{
    std::vector<std::vector<TILE_TYPE>> tiles;

public:
    void setSize(const vec2i size);
    vec2i getSize();

    TILE_TYPE& at(const vec2i pos);

    void fill(const TILE_TYPE ttype);

    void printToFile(const std::string& fname);

    bool isPositionValid(const vec2i pos);

    void setTiles(const rect_i r, const TILE_TYPE tile);

    bool isTileAdjacentTo(const vec2i pos, TILE_TYPE t, bool diag_check);
    bool isTileAdjacentOnlyTo(const vec2i pos, TILE_TYPE t, bool diag_check);

    void generate(const gen_params params);
    void generateHallway(const vec2i start_p, const gen_params params, const int direction);
    void generateRoom(const vec2i start_p, const gen_params params);
    

    unsigned int tileCount(const rect_i r);
    unsigned int tileCount(const rect_i r, const TILE_TYPE ttype);
};