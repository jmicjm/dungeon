#include <iostream>
#include "level_gen/level_structure_gen.h"
#include "level/level_structure.h"

int main()
{
    gen_params g_params;
    g_params.min_room_size = { 2,2 };
    g_params.max_room_size = { 10,10 };
    g_params.min_hallway_segment_length = 2;
    g_params.max_hallway_segment_length = 5;
    g_params.min_hallway_segment_count = 1;
    g_params.max_hallway_segment_count = 5;
    g_params.min_door_count = 2;
    g_params.max_door_count = 5;

    
    vec2i l_size;
    std::cin >> l_size.x >> l_size.y;

    level_structure l_s;
    l_s.setSize(l_size);

    level_structure_gen l_gen(l_s, g_params);
    l_gen.generate();

    l_s.printToFile("mapa.txt");
}