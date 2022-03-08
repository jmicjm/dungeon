#include "level.h"
#include "../entities/entity.h"
#include "../gfx/utils/visibleAreaBounds.h"
#include "../level_gen/createLevelStructure.h"
#include "../utils/sf_vector2_utils.h"


void Level::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    if(tile_map) rt.draw(*tile_map, st);
    rt.draw(door_controller, st);
    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());

    auto pixelPosition = [](const sf::Vector2i& tile_position)
    {
        return vecMul(sf::Vector2f{ tile_position }, Tile_sprite_storage::tile_size);
    };

    auto area_entrances = entrances.find({ tl, br });
    for (auto entrance : area_entrances)
    {
        Primitive_sprite sprite = entrance->second.getSprite();

        sf::RenderStates st2 = st;
        st2.transform.translate(pixelPosition(entrance->first));
        st2.texture = sprite.texture;

        rt.draw(sprite.vertices, 4, sprite.primitive_type, st2);
    }

    auto area_entites = entities.find({ tl, br });
    for (auto entity : area_entites)
    {
        entity->second->update();
        
        sf::RenderStates st2 = st;
        st2.transform.translate(pixelPosition(entity->second->getPosition()) - sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));

        rt.draw(*entity->second, st2);
    }
    
    if(view_range_overlay) rt.draw(*view_range_overlay);
}

Level::Level(const Level_params& params)
    : structure(createLevelStructure(params.structure_params)),
    entities({ { 0,0 }, structure.getSize() }),
    entrances({ { 0,0 }, structure.getSize() }),
    door_controller(this),
    reveal_mask(params.structure_params.level_size) {}

void Level::updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt)
{
    const auto visible_tiles_vec = std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>{ visible_tiles.begin(), visible_tiles.end() };

    reveal_mask.reveal(visible_tiles_vec);
    if(view_range_overlay) view_range_overlay->update(*this, visible_tiles_vec, reveal_mask, rt);
}

const Level_structure& Level::getStructure() const
{
    return structure;
}

void Level::update()
{
    auto entities = this->entities.find(this->entities.getArea());
    for (auto e : entities)
    {
        e->second->performAction();
    }
}

void Level::loadVisuals()
{
    if(!tile_map) tile_map = std::make_unique<Level_tile_map>(structure, sf::Vector2f{ Tile_sprite_storage::tile_size }, sf::Vector2i{ 30,30 });
    if(!view_range_overlay) view_range_overlay = std::make_unique<View_range_overlay>();
}

void Level::unloadVisuals()
{
    tile_map = nullptr;
    view_range_overlay = nullptr;
}
