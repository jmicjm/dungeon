#include "level.h"
#include "../level_gen/level_structure_generator.h"
#include "../level_gen/level_structure_decorator.h"
#include "../entities/entity.h"
#include "../gfx/utils/visibleAreaBounds.h"

void Level::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(tile_map, st);
    rt.draw(door_controller, st);
    const auto [tl, br] = visibleAreaBounds(rt.getView(), tile_size);

    auto area_entites = entities.find({ tl, br + sf::Vector2i{1,1} });
    for (auto entity : area_entites)
    {
        entity->second->updateState(false);
        rt.draw(*entity->second);
    }
    rt.draw(view_range_overlay);
}

void Level::create(const Level_params& params)
{
    Level_structure_generator{}.generate(structure, params.gen_params);
    Level_structure_decorator{}.decorate(structure);
    tile_map.populate(structure, params.tile_size, params.tile_map_chunk_size);
    reveal_mask.create(params.gen_params.level_size);
    tile_size = sf::Vector2i{ params.tile_size };

    door_controller = Door_controller{ this };
    entities = Quadtree<std::shared_ptr<Entity>>({ {0,0}, structure.getSize() });
}