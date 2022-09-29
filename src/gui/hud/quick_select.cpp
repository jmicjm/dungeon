#include "quick_select.h"
#include "../../asset_storage/texture_bank.h"
#include "../../components/inventory.h"
#include "../../components/render_component.h"

void gui::Quick_select::redraw()
{
    bg.draw();

    if (auto inv = registry.try_get<Inventory>(player))
    {
        for (auto idx = 0; idx < 5; idx++)
        {
            if (auto item = inv->get(idx); item != entt::null)
            {
                if (auto rc = registry.try_get<Render_component>(item))
                {
                    sf::RenderStates st;
                    st.transform.translate({ 2 * scale, (size().y - 2 * scale) / 5 * idx + 2 * scale }).scale({ (size().x - 4 * scale) / 64, (size().x - 4 * scale) / 64 });
                    for (auto& [zlevel, animations] : rc->zlevel_animation_map)
                    {
                        for (auto& animation : animations)
                        {
                            animation.updateFrameIdx();
                            draw(animation, st);
                        }
                    }
                }
            }
        }
    }

    fg.draw();
}

gui::Quick_select::Quick_select(entt::registry& registry, entt::entity player)
    : registry(registry), player(player)
{
    bg.setSizeInfo({ {0,0}, {1,1} });
    bg.setParent(this);
    fg.setSizeInfo({ {0,0}, {1,1} });
    fg.setParent(this);

    const auto tex = Texture_bank::getTexture("gui/quick_select.png");
    bg.setAppearance({ sf::Sprite{ *tex, {18,0,18,82} } });;
    fg.setAppearance({ sf::Sprite{ *tex, {0,0,18,82} } });;
}

void gui::Quick_select::setPlayer(entt::entity player)
{
    this->player = player;
}

void gui::Quick_select::setScale(float scale)
{
    this->scale = scale;
}
