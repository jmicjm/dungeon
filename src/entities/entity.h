#pragma once
#include "../level/level.h"
#include "../level/tile_visibility_info.h"
#include "../utils/sf_vector2_hash.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML//Graphics/RenderWindow.hpp"

#include <utility>
#include <unordered_map>

class Entity : public sf::Drawable
{
private:
    sf::Vector2i position;

protected:
    Level* level = nullptr;

    unsigned int vision_radius = 6;

public:
    Entity(Level* level, const sf::Vector2i& position);
    virtual ~Entity() = default;

    void setPosition(const sf::Vector2i& position);
    sf::Vector2i getPosition() const;

    void move(sf::Vector2i& offset);

    virtual void update() = 0;
    virtual bool performAction() = 0;

    std::unordered_map<sf::Vector2i, Tile_visibility_info> getVisibleTiles() const;
};