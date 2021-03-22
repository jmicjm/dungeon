#include <iostream>
#include "map_gen.h"

int main()
{
    gen_params g_params;
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 8,8 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.min_door_count = 2;
    g_params.max_door_count = 5;

    map m;
    vec2i s;
    std::cin >> s.x >> s.y;
    m.setSize(s);

    m.generate(g_params);


    m.printToFile("mapa.txt");

}