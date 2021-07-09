#pragma once
#include "tile.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"

#include <vector>
#include <string>


class level_structure
{
    friend class level_structure_generator;

    std::vector<std::vector<tile>> tiles;
    std::vector<rect_i> room_rects;

    void setSize(const vec2i size);

public:
    vec2i getSize() const;

    tile& at(const vec2i pos);
    const tile& at(const vec2i pos) const;
    bool isPositionValid(const vec2i pos) const;

    size_t roomCount() const;
    rect_i getRoomRect(size_t room_idx) const;

    void printToFile(const std::string& fname) const;
};