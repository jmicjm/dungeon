#include <iostream>
#include "map_gen.h"

int main()
{
    gen_params g_params;
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_length = 1;
    g_params.max_hallway_length = 10;

    map m;
    vec2i s;
    std::cin >> s.x >> s.y;
    m.setSize(s);

    m.generate(g_params);


    m.printToFile("mapa.txt");

}