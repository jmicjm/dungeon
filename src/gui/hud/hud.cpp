#include "hud.h"
#include "../../asset_storage/texture_bank.h"
#include "../../global/gui_component_stack.h"
#include "../inventory/player_inventory.h"
#include "../../world/world.h"
#include "../../components/body/body.h"

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
    quick_select.setScale(scale);

    inventory_button.setSizeInfo({ sf::Vector2f{16,16} * scale });
    inventory_button.setPositionInfo({ sf::Vector2f{2,-2} * scale, {0,0}, {0,1} });

    controls.setPositionInfo({ sf::Vector2f{-1,1} * scale, {0,0}, {1,0}});
}

void gui::Hud::redraw()
{
    player_frame.draw();
    hp_bar.draw();
    mana_bar.draw();
    quick_select.draw();
    inventory_button.draw();
    controls.draw();
}

void gui::Hud::updateBars()
{
    using enum Body_attribute;
    using enum Body_stat;
    using enum Body_part_attribute;
    using enum Body_part_stat;

    if (const auto player_body = world.getRegistry().try_get<Body>(player))
    {
        hp_bar.setValue(player_body->getPartsStat(HP).value_or(0));
        hp_bar.setMaxValue(player_body->getPartsAttribute(world.getRegistry(), MAX_HP).value_or(0));

        mana_bar.setValue(player_body->getStat(MANA).value_or(0));
        mana_bar.setMaxValue(player_body->getAttribute(world.getRegistry(), MAX_MANA).value_or(0));
    }
}

void gui::Hud::activateEvent()
{
    player_frame.activate();
    hp_bar.activate();
    mana_bar.activate();
    quick_select.activate();
    inventory_button.activate();
    controls.activate();
}

void gui::Hud::deactivateEvent()
{
    player_frame.deactivate();
    hp_bar.deactivate();
    mana_bar.deactivate();
    quick_select.deactivate();
    inventory_button.deactivate();
    controls.deactivate();
}

gui::Hud::Hud(World& world, entt::entity player)
    : world(world),
    player(player),
    quick_select(world.getRegistry(), player)
{
    setSizeInfo({ {0,0}, {1,1} });

    const auto frame_tex = Texture_bank::getTexture("gui/player_frame.png");
    player_frame.setAppearance({ sf::Sprite{ *frame_tex } });
    player_frame.setParent(this);

    const auto bar_tex = Texture_bank::getTexture("gui/status_bars.png");
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

    const auto inv_btn_tex = Texture_bank::getTexture("gui/inventory_button.png");
    const Button_appearance inv_btn_a = { .pressed = sf::Sprite{ *inv_btn_tex, { {32,0}, {16,16} } },
                                          .released = sf::Sprite{ *inv_btn_tex, { {0,0}, {16,16} } },
                                          .pressed_hovered = sf::Sprite{ *inv_btn_tex, { {32,0}, {16,16} } },
                                          .released_hovered = sf::Sprite{ *inv_btn_tex, { {16,0}, {16,16} } } };
    inventory_button.setAppearance(inv_btn_a);
    inventory_button.setType(Button::SWITCH);
    inventory_button.setParent(this);

    quick_select.setParent(this);
    quick_select.setAnchor(&inventory_button);


    auto inventory_button_f = [this]
    {
        auto inv = std::make_unique<gui::Player_inventory>(this->world, this->player);
        inv->setSizeInfo({ {0,0}, {0.7,0.9} });
        inv->setPositionInfo({ {0,0}, {0,0}, {0.5,0.5} });    
        Component_stack::Component_config cfg;
        cfg.on_close = [this] 
        {
            activate();
            inventory_button.release();
        };
        gui_component_stack.insert(std::move(inv), cfg);
        deactivate();
    };
    inventory_button.setPressFunction(inventory_button_f);

    player_frame.setPlayer(world.getRegistry(), player);
    controls.setPlayer(world.getRegistry(), player);

    rescale();
}

void gui::Hud::update()
{
    updateBars();

    player_frame.update();
    hp_bar.update();
    mana_bar.update();
    quick_select.update();
    inventory_button.update();
    controls.update();
}

void gui::Hud::setPlayer(entt::entity player)
{
    this->player = player;
    player_frame.setPlayer(world.getRegistry(), player);
    controls.setPlayer(world.getRegistry(), player);
    quick_select.setPlayer(player);
}

void gui::Hud::setScale(float scale)
{
    this->scale = scale;
    controls.setScale(scale * 2 / 3);
    rescale();
}

float gui::Hud::getScale() const
{
    return scale;
}