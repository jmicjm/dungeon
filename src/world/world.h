#pragma once
#include "world_params.h"
#include "../level/level.fwd.h"
#include "../entities/entity.h"
#include "../entities/player.h"

#include "SFML/Graphics/Drawable.hpp"

#include <vector>
#include <memory>


class World : public sf::Drawable
{
    std::vector<std::shared_ptr<Level>> levels;
    std::shared_ptr<Level> current_level;
    std::shared_ptr<Player> player;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    World(const World_params& params, const std::shared_ptr<Player>& player);

    void update(const sf::RenderTarget& rt);
    std::shared_ptr<Level> changeLevel(std::shared_ptr<Level> level);
};