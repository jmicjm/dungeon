#pragma once

enum class TILE_TYPE
{
	WALL, ROOM, HALLWAY
};

struct tile
{
	TILE_TYPE type = TILE_TYPE::WALL;
	/*
	bool revealed;
	sprite etc
	*/
};