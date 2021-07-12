#pragma once
#include "tile.h"
#include "../utils/vec2.h"
#include "../utils/rect.h"

#include <vector>
#include <string>


class Level_structure
{
    friend class Level_structure_generator;

    std::vector<std::vector<Tile>> tiles;
    std::vector<Rect_i> room_rects;

    void setSize(const Vec2i size);

public:
    Vec2i getSize() const;

    Tile& at(const Vec2i pos);
    const Tile& at(const Vec2i pos) const;
    bool isPositionValid(const Vec2i pos) const;

    size_t roomCount() const;
    Rect_i getRoomRect(size_t room_idx) const;

    void printToFile(const std::string& fname) const;
};