#include "controls.h"
#include "../../asset_storage/texture_bank.h"
#include "../../asset_storage/tile_sprite_storage.h"
#include "../../utils/sf_vector2_utils.h"
#include "../../global/window.h"
#include "../../components/position.h"

void gui::Controls::rescale()
{
    setSizeInfo({ sf::Vector2f{22,62} * scale });

    options_button.setSizeInfo({ sf::Vector2f{22,21} * scale });
    skip_button.setSizeInfo({ sf::Vector2f{22,20} * scale });
    center_button.setSizeInfo({ sf::Vector2f{22,21} * scale });
}

void gui::Controls::redraw()
{
    options_button.draw();
    skip_button.draw();
    center_button.draw();
}

void gui::Controls::activateEvent()
{
    options_button.activate();
    skip_button.activate();
    center_button.activate();
}

void gui::Controls::deactivateEvent()
{
    options_button.deactivate();
    skip_button.deactivate();
    center_button.deactivate();
}

gui::Controls::Controls()
{
    options_button.setParent(this);
    skip_button.setParent(this);
    center_button.setParent(this);

    const auto texture = Texture_bank::getTexture("assets/textures/gui/top_right_controls.png");

    options_button.setAppearance({
        .pressed = sf::Sprite{ *texture, sf::IntRect{22,0,22,21} },
        .released = sf::Sprite{ *texture, sf::IntRect{0,0,22,21} },
        .pressed_hovered = sf::Sprite{ *texture, sf::IntRect{22,0,22,21} },
        .released_hovered = sf::Sprite{ *texture, sf::IntRect{22,0,22,21} }
    });

    skip_button.setAppearance({
        .pressed = sf::Sprite{ *texture, sf::IntRect{22,21,22,20} },
        .released = sf::Sprite{ *texture, sf::IntRect{0,21,22,20} },
        .pressed_hovered = sf::Sprite{ *texture, sf::IntRect{22,21,22,20} },
        .released_hovered = sf::Sprite{ *texture, sf::IntRect{22,21,22,20} }
    });
    skip_button.setAnchor(&options_button);
    skip_button.setAnchorPositionInfo({ Anchor_position_info::BOTTOM });

    center_button.setAppearance({
        .pressed = sf::Sprite{ *texture, sf::IntRect{22,41,22,21} },
        .released = sf::Sprite{ *texture, sf::IntRect{0,41,22,21} },
        .pressed_hovered = sf::Sprite{ *texture, sf::IntRect{22,41,22,21} },
        .released_hovered = sf::Sprite{ *texture, sf::IntRect{22,41,22,21} }
    });
    center_button.setAnchor(&skip_button);
    center_button.setAnchorPositionInfo({ Anchor_position_info::BOTTOM });
    
    rescale();
}

void gui::Controls::update()
{
    options_button.update();
    skip_button.update();
    center_button.update();
}

void gui::Controls::setPlayer(const entt::registry& registry, entt::entity player)
{
    center_button.setPressFunction([&, player] {
        auto view = window.getView();
        view.setCenter(
            sf::Vector2f{ vecMul(registry.get<Position>(player).getCoords(), Tile_sprite_storage::tile_size) + sf::Vector2i{32, 0}}
        ); 
        window.setView(view);
    });
}

void gui::Controls::setScale(float scale)
{
    this->scale = scale;
    rescale();
}

float gui::Controls::getScale() const
{
    return scale;
}
