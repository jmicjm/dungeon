#include "level_structure_decorator.h"
#include "../asset_storage/tile_sprite_storage.h"
#include "../utils/rand.h"


TILE_SPRITE_ID::tile_sprite_id_t level_structure_decorator::getTileSpriteId(const vec2i pos)
{
	TILE_SPRITE_ID::tile_sprite_id_t id;
	const TILE_TYPE ttype = ls->at(pos).type;
	switch (ttype)
	{
	case TILE_TYPE::HALLWAY:
	case TILE_TYPE::ROOM:
		id = TILE_SPRITE_ID::FLOOR;
		break;
	case TILE_TYPE::WALL:
		id = TILE_SPRITE_ID::WALL;
	}
	if (ls->isPositionValid(pos + vec2i{ 0, -1 }))
	{
		if (ls->at(pos + vec2i{ 0, -1 }).type != ttype)
		{
			id |= TILE_SPRITE_ID::T;
		}
	}
	if (ls->isPositionValid(pos + vec2i{ -1, 0 }))
	{
		if (ls->at(pos + vec2i{ -1, 0 }).type != ttype)
		{
			id |= TILE_SPRITE_ID::L;
		}
	}
	if (ls->isPositionValid(pos + vec2i{ 0, 1 }))
	{
		if (ls->at(pos + vec2i{ 0, 1 }).type != ttype)
		{
			id |= TILE_SPRITE_ID::B;
		}
	}
	if (ls->isPositionValid(pos + vec2i{ 1, 0 }))
	{
		if (ls->at(pos + vec2i{ 1, 0 }).type != ttype)
		{
			id |= TILE_SPRITE_ID::R;
		}
	}
	if (ttype == TILE_TYPE::WALL)
	{
		if (ls->isPositionValid(pos + vec2i{ -1, -1 }))
		{
			if (ls->at(pos + vec2i{ -1, -1 }).type != ttype)
			{
				id |= TILE_SPRITE_ID::TL;
			}
		}
		if (ls->isPositionValid(pos + vec2i{ 1, -1 }))
		{
			if (ls->at(pos + vec2i{ 1, -1 }).type != ttype)
			{
				id |= TILE_SPRITE_ID::TR;
			}
		}
		if (ls->isPositionValid(pos + vec2i{ -1, 1 }))
		{
			if (ls->at(pos + vec2i{ -1, 1 }).type != ttype)
			{
				id |= TILE_SPRITE_ID::BL;
			}
		}
		if (ls->isPositionValid(pos + vec2i{ 1, 1 }))
		{
			if (ls->at(pos + vec2i{ 1, 1 }).type != ttype)
			{
				id |= TILE_SPRITE_ID::BR;
			}
		}
	}

	return id;
}

void level_structure_decorator::setLevelStructure(level_structure& l)
{
	ls = &l;
}

void level_structure_decorator::decorate()
{
	for (int x = 0; x < ls->getSize().x; x++)
	{
		for (int y = 0; y < ls->getSize().y; y++)
		{
			TILE_SPRITE_ID::tile_sprite_id_t id = getTileSpriteId({ x,y });
			if (ls->at({ x,y }).type != TILE_TYPE::WALL)
			{
				if (rand(0, 1))
				{
					id |= TILE_SPRITE_ID::ROCK;
				}
				else
				{
					id |= TILE_SPRITE_ID::WOOD;
				}
			}
			else
			{
				id |= TILE_SPRITE_ID::ROCK;
			}

			ls->at({ x,y }).id = id;

			std::vector<sf::Sprite>* sprites = tile_sprite_storage::getSprite(id);
			if (sprites == nullptr)
			{
				using namespace TILE_SPRITE_ID;
				sprites = tile_sprite_storage::getSprite(id & ~(TL|TR|BL|BR));//fallback to generic
				if (sprites == nullptr)
				{
					sprites = tile_sprite_storage::getSprite(id & ~(TL|T|TR|R|BL|B|BR|L));//fallback to generic
				}
			}
			if (sprites != nullptr && sprites->size() > 0)
			{
				unsigned int variant = rand(0, sprites->size() - 1);

				ls->at({ x,y }).sprite = &(*sprites)[variant];
				ls->at({ x,y }).id_variant = variant;
			}	
		}
	}
}