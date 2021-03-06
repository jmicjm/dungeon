#pragma once
#include "../level/level_structure.h"
#include "../utils/rect.h"
#include "level_structure_params.h"


class Level_structure_generator
{
    Level_structure* ls = nullptr;
    Level_structure_params params;

    void fill(const Tile tile);
    void setTiles(Rect_i r, const TILE_TYPE ttype);

    bool generateHallway(const sf::Vector2i& start_p);
    bool generateRoom(const sf::Vector2i& start_p);

    void fillEmptyAreas();
    bool fillEmptyArea(Rect_i area);

    void clipRectToLevelSize(Rect_i& r);

    enum ADJACENT_AREA : uint8_t
    {
        AXIS = 1,
        DIAG = 1 << 1
    };
    template<typename T>
    unsigned int adjacentTileCount(const sf::Vector2i& pos, const uint8_t area, const T& pred);
    unsigned int adjacentTileCount(const sf::Vector2i& pos, const uint8_t area);
    unsigned int adjacentTileCount(const sf::Vector2i& pos, const uint8_t area, const TILE_TYPE ttype);

    template<typename T>
    bool contains(Rect_i area, const T& pred);

public:
    void generate(Level_structure& l, Level_structure_params p) &&;
};