#pragma once
#include <cstdint>

namespace TILE_SPRITE_ID
{
    using tile_sprite_id_t = uint32_t;

    constexpr unsigned int surroundings_offset = 0;
    constexpr tile_sprite_id_t surroundings_mask = 0xFF;
    enum SURROUNDINGS : tile_sprite_id_t
    {
        /*
        TL | T | TR  
        ---|---|---
        L  | X |  R
        ---|---|---
        BL | B | BR  
        */
        L  = 1,
        B  = 1 << 1,
        R  = 1 << 2,
        T  = 1 << 3,

        TL = 1 << 4,
        TR = 1 << 5,
        BR = 1 << 6,
        BL = 1 << 7
    };

    constexpr unsigned int surface_offset = 8;
    constexpr tile_sprite_id_t surface_mask = 0xF << surface_offset;
    enum SURFACE : tile_sprite_id_t
    {
        WALL    = 1 << surface_offset,
        FLOOR   = 2 << surface_offset,
        HALLWAY = 3 << surface_offset,
        DOORWAY = 4 << surface_offset
    };

    constexpr unsigned int type_offset = 12;
    constexpr tile_sprite_id_t type_mask = 0xFFF << type_offset;
    enum TYPE : tile_sprite_id_t
    {
        ROCK   = 1 << type_offset,
        WOOD   = 2 << type_offset,
        CARPET = 3 << type_offset
    };

    constexpr unsigned int private_use_offset = 24;
    constexpr tile_sprite_id_t private_use_mask = 0xFF << private_use_offset;
}