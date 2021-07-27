#pragma once 
#include "SFML/System/Vector2.hpp"

struct Tile_visibility_info
{
    sf::Vector2i position;
    bool tl, tr, bl, br;
    bool visible() const { return tl || tr || bl || br; }
};

inline bool operator==(const Tile_visibility_info& a, const Tile_visibility_info& b)
{
    return a.position == b.position
        && a.tl == b.tl
        && a.tr == b.tr
        && a.bl == b.bl
        && a.br == b.br;
}