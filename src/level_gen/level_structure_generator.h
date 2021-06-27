#pragma once
#include "../level/level_structure.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"
#include "gen_params.h"


class level_structure_generator
{
    level_structure* ls = nullptr;
    gen_params params;

    void fill(const tile t);
    void setTiles(const rect_i r, const TILE_TYPE ttype);

    bool generateHallway(const vec2i start_p);
    bool generateRoom(const vec2i start_p);

    void fillEmptyAreas();
    void fillEmptyArea(rect_i area);

    template<typename T>
    unsigned int tileCount(const rect_i r, const T& pred);
    unsigned int tileCount(const rect_i r);
    unsigned int tileCount(const rect_i r, const TILE_TYPE ttype);

    enum ADJACENT_AREA : uint8_t
    {
        AXIS = 1,
        DIAG = 1 << 1
    };
    template<typename T>
    unsigned int adjacentTileCount(const vec2i pos, const uint8_t area, const T& pred);
    unsigned int adjacentTileCount(const vec2i pos, const uint8_t area);
    unsigned int adjacentTileCount(const vec2i pos, const uint8_t area, const TILE_TYPE ttype);

public:
    void generate(level_structure& l, gen_params p);
};