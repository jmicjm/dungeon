#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../../entities/player.h"

#include <memory>


namespace gui
{
    class Player_frame : public Gui_component
    {
        std::shared_ptr<Player> player;
        Frame frame;

        void redraw() override;

    public:
        Player_frame(sf::RenderWindow* rw);

        void setPlayer(std::shared_ptr<Player> player);
        void setAppearance(const Frame_appearance& appearance);
    };
}
