#include "level.h"
#include "../level_gen/level_structure_generator.h"
#include "../level_gen/level_structure_decorator.h"

void Level::create(const Level_params& params)
{
    Level_structure_generator{}.generate(ls, params.gen_params);
    Level_structure_decorator{}.decorate(ls);
    tmap.populate(ls, params.tile_size, params.tile_map_chunk_size);
}