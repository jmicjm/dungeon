#include "hud.h"
#include "../../asset_storage/texture_bank.h"

#include "SFML/Graphics/Sprite.hpp"


void gui::Hud::rescale()
{
    player_frame.setSizeInfo({ sf::Vector2f{53,61} * scale });
    player_frame.setPositionInfo({ sf::Vector2f{1,1} * scale });

    hp_bar.setSizeInfo({ sf::Vector2f{60,10} * scale });
    hp_bar.setAnchorPositionInfo({ Anchor_position_info::BOTTOM, sf::Vector2f{-1,1} * scale, 0 });

    mana_bar.setSizeInfo({ sf::Vector2f{60,10} * scale });
    mana_bar.setAnchorPositionInfo({ Anchor_position_info::BOTTOM, sf::Vector2f{0,1} * scale, 0 });

    quick_select.setSizeInfo({ sf::Vector2f{18,82} * scale });
    quick_select.setAnchorPositionInfo({ .offset = sf::Vector2f{0,-2} * scale });

    inventory_button.setSizeInfo({ sf::Vector2f{16,16} * scale });
    inventory_button.setPositionInfo({ sf::Vector2f{2,-2} * scale, {0,0}, {0,1} });
}

void gui::Hud::redraw()
{
    player_frame.draw();
    hp_bar.draw();
    mana_bar.draw();
    quick_select.draw();
    inventory_button.draw();
}

void gui::Hud::activateEvent()
{
    player_frame.activate();
    hp_bar.activate();
    mana_bar.activate();
    quick_select.activate();
    inventory_button.activate();
}

void gui::Hud::deactivateEvent()
{
    player_frame.deactivate();
    hp_bar.deactivate();
    mana_bar.deactivate();
    quick_select.deactivate();
    inventory_button.deactivate();
}

gui::Hud::Hud(sf::RenderWindow* rw)
    : Gui_component(rw), player_frame(rw), hp_bar(rw), mana_bar(rw), quick_select(rw), inventory_button(rw)
{
    setSizeInfo({ {0,0}, {1,1} });

    const auto frame_tex = Texture_bank::getTexture("assets/gui/player_frame.png");
    player_frame.setAppearance({ sf::Sprite{ *frame_tex } });
    player_frame.setParent(this);

    const auto bar_tex = Texture_bank::getTexture("assets/gui/status_bars.png");
    const sf::Sprite hp_bar_frame_s{ *bar_tex, { {0,0}, {60,10} } };
    const sf::Sprite hp_bar_s{ *bar_tex, { {0,10}, {50,6} } };
    const sf::Sprite mana_bar_frame_s{ *bar_tex, { {0,20}, {60,10} } };
    const sf::Sprite mana_bar_s{ *bar_tex, { {0,30}, {50,6} } };
    hp_bar.setAppearance({ hp_bar_frame_s }, { hp_bar_s });  
    hp_bar.setParent(this);
    hp_bar.setAnchor(&player_frame);
    mana_bar.setAppearance({ mana_bar_frame_s }, { mana_bar_s });
    mana_bar.setParent(this);
    mana_bar.setAnchor(&hp_bar);

    hp_bar.setMinValue(0);
    hp_bar.setMaxValue(100);
    hp_bar.setValue(100);
    mana_bar.setMinValue(0);
    mana_bar.setMaxValue(100);
    mana_bar.setValue(100);

    const auto inv_btn_tex = Texture_bank::getTexture("assets/gui/inventory_button.png");
    const Button_appearance inv_btn_a = { .pressed = sf::Sprite{ *inv_btn_tex, { {32,0}, {16,16} } },
                                          .released = sf::Sprite{ *inv_btn_tex, { {0,0}, {16,16} } },
                                          .pressed_hovered = sf::Sprite{ *inv_btn_tex, { {32,0}, {16,16} } },
                                          .released_hovered = sf::Sprite{ *inv_btn_tex, { {16,0}, {16,16} } } };
    inventory_button.setAppearance(inv_btn_a);
    inventory_button.setParent(this);
    inventory_button.setType(Button::SWITCH);

    const auto qselect_tex = Texture_bank::getTexture("assets/gui/quick_select.png");
    quick_select.setAppearance({ sf::Sprite{ *qselect_tex } });
    quick_select.setParent(this);
    quick_select.setAnchor(&inventory_button);

    rescale();
}

void gui::Hud::update()
{
    player_frame.update();
    hp_bar.update();
    mana_bar.update();
    quick_select.update();
    inventory_button.update();
}

void gui::Hud::setPlayer(std::shared_ptr<Player> player)
{
    this->player = player;
    player_frame.setPlayer(player);
}

void gui::Hud::setScale(float scale)
{
    this->scale = scale;
    rescale();
}

float gui::Hud::getScale() const
{
    return scale;
}