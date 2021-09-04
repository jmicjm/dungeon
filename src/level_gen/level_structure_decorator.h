#pragma once
#include "../level/level_structure.h"
#include "../utils/rect.h"


class Level_structure_decorator
{
    Level_structure* ls = nullptr;

    TILE_SPRITE_ID::tile_sprite_id_t getSurroundingsId(const sf::Vector2i& pos);
   // TILE_SPRITE_ID::tile_sprite_id_t get...Id(const vec2i pos, const std::vector<vec2i>& pattern);

    bool addSprite(const sf::Vector2i& pos, TILE_SPRITE_ID::tile_sprite_id_t id);

    void placeWalls();
    void placeWall(const sf::Vector2i& pos);
    void placeFloors();
    void placeCarpets();
    void placeCarpet(const Rect_i& area);

public:
    void decorate(Level_structure& l);
};