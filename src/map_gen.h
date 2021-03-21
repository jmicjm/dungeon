#pragma once
#include "vec2.h"
#include "rect.h"

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

    void generate(vec2i max_room_size, unsigned int max_hallway_length);
    void generateHallway(const vec2i start_p, const unsigned int max_length, vec2i max_room_size, int direction);
    void generateRoom(const vec2i start_p, const vec2i max_room_size, unsigned int max_hallway_length);
    

    unsigned int tileCount(const rect_i r);
    unsigned int tileCount(const rect_i r, const TILE_TYPE ttype);
};