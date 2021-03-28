#pragma once
#include "../utils/vec2.h"
#include "../utils/rect.h"

#include <vector>
#include <string>

enum class TILE_TYPE
{
    WALL, ROOM, HALLWAY
};

class level_structure
{
    std::vector<std::vector<TILE_TYPE>> tiles;
public:
    void setSize(const vec2i size);
    vec2i getSize();

    TILE_TYPE& at(const vec2i pos);
    bool isPositionValid(const vec2i pos);

    void printToFile(const std::string& fname);
};