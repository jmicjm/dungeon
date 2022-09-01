#pragma once
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "entt/entt.hpp"


namespace gui
{
    void drawItem(sf::RenderTarget& rt, sf::RenderStates st, entt::registry& registry, entt::entity item);
}