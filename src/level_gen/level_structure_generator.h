#pragma once
#include "../level/level_structure.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"
#include "gen_params.h"


class Level_structure_generator
{
    Level_structure* ls = nullptr;
    Gen_params params;

    void fill(const Tile t);
    void setTiles(const Rect_i r, const TILE_TYPE ttype);

    bool generateHallway(const Vec2i start_p);
    bool generateRoom(const Vec2i start_p);

    void fillEmptyAreas();
    void fillEmptyArea(Rect_i area);

    template<typename T>
    unsigned int TileCount(Rect_i r, const T& pred);
    unsigned int TileCount(Rect_i r);
    unsigned int TileCount(const Rect_i r, const TILE_TYPE ttype);
    void clipRectToLevelSize(Rect_i& r);

    enum ADJACENT_AREA : uint8_t
    {
        AXIS = 1,
        DIAG = 1 << 1
    };
    template<typename T>
    unsigned int adjacentTileCount(const Vec2i pos, const uint8_t area, const T& pred);
    unsigned int adjacentTileCount(const Vec2i pos, const uint8_t area);
    unsigned int adjacentTileCount(const Vec2i pos, const uint8_t area, const TILE_TYPE ttype);

public:
    void generate(Level_structure& l, Gen_params p);
};