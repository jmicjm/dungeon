#include "level.h"
#include "../entities/entity.h"
#include "../gfx/utils/visibleAreaBounds.h"
#include "../level_gen/createLevelStructure.h"

void Level::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(tile_map, st);
    rt.draw(door_controller, st);
    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());

    auto area_entites = entities.find({ tl, br });
    for (auto entity : area_entites)
    {
        entity->second->updateState(false);
        
        const auto [px, py] = entity->second->getPosition();
        const auto [tx, ty] = Tile_sprite_storage::tile_size;
        const sf::Vector2f pos(px * tx, py * ty - ty / 2);
        sf::RenderStates st2 = st;
        st2.transform.translate(pos);

        rt.draw(*entity->second, st2);
    }
    rt.draw(view_range_overlay);
}

Level::Level(const Level_params& params) 
    : structure(createLevelStructure(params.gen_params)),
    entities({ { 0,0 }, structure.getSize() }),
    door_controller(this),
    reveal_mask(params.gen_params.level_size),
    tile_map(structure, sf::Vector2f{ Tile_sprite_storage::tile_size }, params.tile_map_chunk_size) {}

void Level::updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt)
{
    const auto visible_tiles_vec = std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>{ visible_tiles.begin(), visible_tiles.end() };

    reveal_mask.reveal(visible_tiles_vec);
    view_range_overlay.update(*this, visible_tiles_vec, reveal_mask, rt);
}

const Level_structure& Level::getStructure() const
{
    return structure;
}
