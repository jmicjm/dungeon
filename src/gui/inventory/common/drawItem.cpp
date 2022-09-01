#include "drawItem.h"
#include "../../../components/render_component.h"
#include "../../../components/stackable_item.h"
#include "../../../asset_storage/font_bank.h"
#include"SFML/Graphics/Text.hpp"


void gui::drawItem(sf::RenderTarget& rt, sf::RenderStates st, entt::registry& registry, entt::entity item)
{
    if (auto rc = registry.try_get<Render_component>(item))
    {
        for (auto& [zlevel, animations] : rc->zlevel_animation_map)
        {
            for (auto& animation : animations)
            {
                animation.updateFrameIdx();
                rt.draw(animation, st);
            }
        }
    }
    if (auto si = registry.try_get<Stackable_item>(item))
    {
        sf::Text text;
        text.setFont(*Font_bank::getFont("font.ttf"));
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setString(std::to_string(si->getAmount()) + "/" + std::to_string(si->getMaxAmount()));
        const auto lbounds = text.getLocalBounds();
        text.setOrigin(lbounds.left + lbounds.width, lbounds.top + lbounds.height);
        text.setPosition({64,64});
        rt.draw(text, st);
    }
}
