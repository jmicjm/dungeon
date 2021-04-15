#pragma once
#include "../level/level_structure.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"
#include "gen_params.h"

#include <vector>
#include <string>
#include <functional>


class level_structure_generator
{
    level_structure* ls = nullptr;
    gen_params params;

    void fill(const tile t);
    void setTiles(const rect_i r, const TILE_TYPE tile);

    void generateHallway(const vec2i start_p);
    bool generateRoom(const vec2i start_p);

    unsigned int tileCount(const rect_i r);
    unsigned int tileCount(const rect_i r, const TILE_TYPE ttype);

    enum ADJACENT_AREA : uint8_t
    {
        AXIS = 0b01,
        DIAG = 0b10
    };
    unsigned int adjacentTileCount(const vec2i pos, const uint8_t area, const TILE_TYPE ttype);

public:
    level_structure_generator() {}
    level_structure_generator(level_structure& ls, gen_params params) : ls(&ls), params(params) {}

    void setLevelStructure(level_structure& l);
    void setGenParams(gen_params p);  

    void generate();
};