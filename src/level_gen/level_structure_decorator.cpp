#include "level_structure_decorator.h"
#include "../asset_storage/tile_sprite_storage.h"
#include "../utils/rand.h"

#include <numeric>
#include <tuple>


using namespace TILE_SPRITE_ID;

tile_sprite_id_t Level_structure_decorator::getSurroundingsId(const sf::Vector2i& pos)
{
    auto isSame = [&](const sf::Vector2i& pos, const TILE_TYPE ttype)
    {
        if (ls->isPositionValid(pos))
        {
            return ls->at(pos).type == ttype;
        }
        return true;
    };

    tile_sprite_id_t id = 0;
    const TILE_TYPE ttype = ls->at(pos).type;

    id |= !isSame(pos + sf::Vector2i{  0, -1 }, ttype) * T;
    id |= !isSame(pos + sf::Vector2i{  1,  0 }, ttype) * R;
    id |= !isSame(pos + sf::Vector2i{  0,  1 }, ttype) * B;
    id |= !isSame(pos + sf::Vector2i{ -1,  0 }, ttype) * L;

    id |= !isSame(pos + sf::Vector2i{ -1, -1 }, ttype) * TL;
    id |= !isSame(pos + sf::Vector2i{  1, -1 }, ttype) * TR;
    id |= !isSame(pos + sf::Vector2i{  1,  1 }, ttype) * BR;
    id |= !isSame(pos + sf::Vector2i{ -1,  1 }, ttype) * BL;

    return id;
}

bool Level_structure_decorator::addSprite(const sf::Vector2i& pos, TILE_SPRITE_ID::tile_sprite_id_t id)
{
    auto sprites = Tile_sprite_storage::getSprite(id);
    if (sprites == nullptr)
    {	
        sprites = Tile_sprite_storage::getSprite(id &= ~(TL | TR | BL | BR));//fallback to generic
        if (sprites == nullptr)
        {
            sprites = Tile_sprite_storage::getSprite(id &= ~(TL | T | TR | R | BL | B | BR | L));//fallback to generic
        }
    }
    if (sprites != nullptr && sprites->size() > 0)
    {
        const float total_sum_chance = std::accumulate(
            sprites->begin(),
            sprites->end(),
            0.f,
            [](float a, const auto& b) {return a + b.chance; }
        );
        const float chance = rand(0.f, total_sum_chance);

        const unsigned int id_variant = [&]()
        {
            for (auto [sum_chance, i] = std::tuple{ 0.f,0 }; i < sprites->size(); i++)
            {
                sum_chance += (*sprites)[i].chance;
                if (chance <= sum_chance) return i;
            }
        }();

        ls->at(pos).sprites_info.push_back({ id, id_variant});

        return true;
    }
    return false;
}

void Level_structure_decorator::placeWalls()
{
    for (int x = 0; x < ls->getSize().x; x++)
    {
        for (int y = 0; y < ls->getSize().y; y++)
        {
            if (ls->at({ x,y }).type == TILE_TYPE::WALL)
            {
                placeWall({ x,y });
            }
        }
    }
}

void Level_structure_decorator::placeWall(const sf::Vector2i& pos)
{
    const tile_sprite_id_t ids[] =
    {
        T|B|L|R,
        T|B|L,
        T|B|R,
        T|L|R,
        B|L|R,
        T|L,
        T|R,
        B|L,
        B|R,
        T,
        B,
        L,
        R,
        TL,
        TR,
        BL,
        BR
    };

    const std::pair<tile_sprite_id_t, tile_sprite_id_t> neighbours[] =
    {
        {T,  TL|TR},
        {B,  BL|BR},
        {L,  TL|BL},
        {R,  TR|BR},
        {TL, T|L  },
        {TR, T|R  },
        {BL, B|L  },
        {BR, B|R  }
    };

    tile_sprite_id_t s_id = getSurroundingsId(pos);

    for (const auto id : ids)
    {
        if ((s_id & id) == id)
        {
            addSprite(pos, ROCK|WALL|id);
            s_id &= ~id;

            for (const auto& n : neighbours)
            {
                if (id & n.first) s_id &= ~n.second;
            }
        }
    }
}

void Level_structure_decorator::placeFloors()
{
    for (int x = 0; x < ls->getSize().x; x++)
    {
        for (int y = 0; y < ls->getSize().y; y++)
        {        
            if (ls->at({ x,y }).type != TILE_TYPE::WALL)
            {
                const tile_sprite_id_t id = FLOOR | ROCK | getSurroundingsId({ x,y });
                addSprite({ x,y }, id);
            }  
        }
    }

    for (int i = 0; i < ls->roomCount(); i++)
    {
        if (rand(0, 1))
        {
            Rect_i r = ls->getRoomRect(i);
            for (int x = r.tl.x; x <= r.br.x; x++)
            {
                for (int y = r.tl.y; y <= r.br.y; y++)
                {
                    const tile_sprite_id_t id = FLOOR | WOOD | getSurroundingsId({ x,y });
                    addSprite({ x,y }, id);
                }
            }
        }
    }
}

void Level_structure_decorator::placeCarpets()
{
    for (int i = 0; i < ls->roomCount(); i++)
    {
        if (!rand(0, 10))
        {
            placeCarpet(ls->getRoomRect(i));
        }
    }
}

void Level_structure_decorator::placeCarpet(const Rect_i& area)
{
    for (int x = area.tl.x; x <= area.br.x; x++)
    {
        for (int y = area.tl.y; y <= area.br.y; y++)
        {
            const tile_sprite_id_t id = CARPET | FLOOR | getSurroundingsId({ x,y });
            addSprite({ x,y }, id);
        }
    }
}

void Level_structure_decorator::decorate(Level_structure& l)
{
    ls = &l;
 
    placeWalls();
    placeFloors();
    placeCarpets();  
}