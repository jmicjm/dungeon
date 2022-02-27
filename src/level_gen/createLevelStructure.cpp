#include "createLevelStructure.h"
#include "level_structure_generator.h"
#include "level_structure_decorator.h"

Level_structure createLevelStructure(const Level_structure_params& params)
{
	Level_structure structure;
	Level_structure_generator{}.generate(structure, params);
	Level_structure_decorator{}.decorate(structure);

	return structure;
}
