#pragma once
#include "door.h"
#include "level.fwd.h"
#include "../utils/quadtree.h"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class Door_controller : public sf::Drawable
{
    Quadtree<Door> doors;
    const Level* level = nullptr;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    Door_controller(const Level* level);

    const Quadtree<Door>& getDoors() const;
    bool openDoor(const sf::Vector2i& pos);
    bool closeDoor(const sf::Vector2i& pos);
};