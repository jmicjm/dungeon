{ 
	ROCK|FLOOR, //tile id
	{ 
		{"stone_floor_1.png", sf::IntRect(0,0,64,64)}, //tile id variant
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
	WOOD|FLOOR|L,
	{
		{"wooden_floor_left.png", sf::IntRect(0,0,64,64)}
	}
},
{
	WOOD|FLOOR|R,
	{
		{"wooden_floor_right.png", sf::IntRect(0,0,64,64)}
	}
}