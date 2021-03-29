{ 
	ROCK|FLOOR, //tile id
	{ 
		{"stone_floor_1.png", sf::IntRect(0,0,64,64)}, //tile id variety
		{"stone_floor_2.png", sf::IntRect(0,0,64,64)} 
	}
},
{ 
	WOOD|FLOOR,
	{ 
		{"wooden_floor.png", sf::IntRect(0,  0,64,64)},
		{"wooden_floor.png", sf::IntRect(64, 0,64,64)},
		{"wooden_floor.png", sf::IntRect(128,0,64,64)},
		{"wooden_floor.png", sf::IntRect(196,0,64,64)}
	}
},
{
	WOOD|FLOOR|TL|L|BL,
	{
		{"wooden_floor_left.png", sf::IntRect(0,0,64,64)}
	}
},
{
	WOOD|FLOOR|TR|R|BR,
	{
		{"wooden_floor_right.png", sf::IntRect(0,0,64,64)}
	}
}