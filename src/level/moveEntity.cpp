#include "moveEntity.h"


void moveEntity(Quadtree<entt::entity>& entities, Position& position, sf::Vector2i& offset)
{
    auto& coords = position.getCoords();
    auto& level = *position.getLevel();

    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_coords = coords + offset;
    const sf::Vector2i old_coords = coords;

    if (new_coords == old_coords) return;

    const bool move_allowed_structure = [&]
    {
        const sf::Vector2i new_pos_x = coords + sf::Vector2i{ offset.x, 0 };
        const sf::Vector2i new_pos_y = coords + sf::Vector2i{ 0, offset.y };

        return !(level.getStructure().at(new_pos_x).type == TILE_TYPE::WALL && level.getStructure().at(new_pos_y).type == TILE_TYPE::WALL)
            && level.getStructure().at(new_coords).type != TILE_TYPE::WALL;
    }();

    if (move_allowed_structure)
    {
        const bool move_allowed_door = [&]
        {
            if (level.getStructure().at(new_coords).type == TILE_TYPE::DOORWAY) return level.door_controller.openDoor(new_coords);
            return true;
        }();

        if (move_allowed_door)
        {
            position.setCoords(new_coords);
            if (level.getStructure().at(old_coords).type == TILE_TYPE::DOORWAY) level.door_controller.closeDoor(old_coords, entities);
        }
    }
}