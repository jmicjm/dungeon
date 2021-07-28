#include "tile_visibility_info.h"

bool Tile_visibility_info::isVisible() const 
{
    return tl || tr || bl || br; 
}

bool Tile_visibility_info::isFullyVisible() const 
{ 
    return tl && tr && bl && br;
}

Tile_visibility_info::Tile_visibility_info()
    : tl(false), tr(false), bl(false), br(false) {}

Tile_visibility_info::Tile_visibility_info(bool tl, bool tr, bool bl, bool br)
    : tl(tl), tr(tr), bl(bl), br(br) {}

void Tile_visibility_info::combine(Tile_visibility_info other)
{
    tl |= other.tl;
    tr |= other.tr;
    bl |= other.bl;
    br |= other.br;
}


bool operator==(const Tile_visibility_info& a, const Tile_visibility_info& b)
{
    return a.tl == b.tl
        && a.tr == b.tr
        && a.bl == b.bl
        && a.br == b.br;
}