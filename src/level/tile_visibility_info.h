#pragma once 
#include "SFML/System/Vector2.hpp"

struct Tile_visibility_info
{
    bool tl, tr, bl, br;

    bool isVisible() const;
    bool isFullyVisible() const;
    Tile_visibility_info();
    Tile_visibility_info(bool tl, bool tr, bool bl, bool br);

    void combine(Tile_visibility_info other);
};

bool operator==(const Tile_visibility_info& a, const Tile_visibility_info& b);