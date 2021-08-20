#pragma once
#include "door.h"
#include "level.fwd.h"
#include "../utils/quadtree.h"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"


class Door_controller : public sf::Drawable
{
    std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBounds(const sf::View& view) const;
public:
    Level* level = nullptr;
    Quadtree<Door> doors;

    void populate();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    Door_controller(Level* level);
    void update(const sf::View& view);
};