#include "level_structure_decorator.h"
#include "../asset_storage/tile_sprite_storage.h"
#include "../utils/rand.h"


TILE_SPRITE_ID::tile_sprite_id_t Level_structure_decorator::getTileSpriteId(const Vec2i pos)
{
	auto isSame = [&](const Vec2i pos, const TILE_TYPE ttype)
	{
		if (ls->isPositionValid(pos))
		{
			return ls->at(pos).type == ttype;
		}
		return true;
	};

	using namespace TILE_SPRITE_ID;

	tile_sprite_id_t id;
	const TILE_TYPE ttype = ls->at(pos).type;
	switch (ttype)
	{
	case TILE_TYPE::HALLWAY:
	case TILE_TYPE::DOORWAY:
	case TILE_TYPE::ROOM:
		id = TILE_SPRITE_ID::FLOOR;
		break;
	case TILE_TYPE::WALL:
		id = TILE_SPRITE_ID::WALL;
	}

	id |= !isSame(pos + Vec2i{  0, -1 }, ttype) * T;
	id |= !isSame(pos + Vec2i{  1,  0 }, ttype) * R;
	id |= !isSame(pos + Vec2i{  0,  1 }, ttype) * B;
	id |= !isSame(pos + Vec2i{ -1,  0 }, ttype) * L;

	id |= !isSame(pos + Vec2i{ -1, -1 }, ttype) * TL;
	id |= !isSame(pos + Vec2i{  1, -1 }, ttype) * TR;
	id |= !isSame(pos + Vec2i{  1,  1 }, ttype) * BR;
	id |= !isSame(pos + Vec2i{ -1,  1 }, ttype) * BL;

	return id;
}

bool Level_structure_decorator::addSprite(const Vec2i pos, TILE_SPRITE_ID::tile_sprite_id_t id)
{
	using namespace TILE_SPRITE_ID;

	std::vector<Tile_sprite_data>* sprites = Tile_sprite_storage::getSprite(id);
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
		unsigned int variant = rand(0, sprites->size() - 1);
		ls->at(pos).sprites_info.push_back({ id, variant});

		return true;
	}
	return false;
}

void Level_structure_decorator::placeCarpet(const Rect_i area)
{
	for (int x = area.tl.x; x <= area.br.x; x++)
	{
		for (int y = area.tl.y; y <= area.br.y; y++)
		{
			using namespace TILE_SPRITE_ID;

			TILE_SPRITE_ID::tile_sprite_id_t id = getTileSpriteId({ x,y });
			id |= OVERLAY | CARPET;

			addSprite({ x,y }, id);
		}
	}
}

void Level_structure_decorator::decorate(Level_structure& l)
{
	ls = &l;

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

			addSprite({ x,y }, id);
		}	
	}
	//place carpets in random rooms
	for (int i = 0; i < ls->roomCount(); i++)
	{
		if (!rand(0, 10))
		{
			placeCarpet(ls->getRoomRect(i));
		}
	}
}