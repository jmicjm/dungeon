#pragma once
#include "door.h"
#include "level.fwd.h"
#include "../utils/quadtree.h"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class Door_controller : public sf::Drawable
{
    const Level* level = nullptr;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
public:
    Quadtree<Door> doors;

    Door_controller(const Level* level);
    void update(const sf::View& view);
};