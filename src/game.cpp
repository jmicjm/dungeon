#include <iostream>
#include "map_gen.h"

int main()
{
    vec2i max_room_size = { 10,10 };
    unsigned int max_hallway_length = 20;

    map m;
    vec2i s;
    std::cin >> s.x >> s.y;
    m.setSize(s);

    m.generate(max_room_size, max_hallway_length);


    m.printToFile("mapa.txt");

}