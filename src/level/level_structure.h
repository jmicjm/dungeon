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
    friend class level_structure_gen;

    std::vector<std::vector<TILE_TYPE>> tiles;
    std::vector<rect_i> room_rects;

public:
    void setSize(const vec2i size);
    vec2i getSize();

    TILE_TYPE& at(const vec2i pos);
    bool isPositionValid(const vec2i pos);

    size_t roomCount();
    rect_i getRoomRect(size_t room_idx);

    void printToFile(const std::string& fname);
};