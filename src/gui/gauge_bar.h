#pragma once
#include "gui_element.h"
#include "surface.h"

#include "SFML/Graphics/RenderTexture.hpp"

namespace gui
{
    class Gauge_bar : public Gui_element
    {
        float min_value = 0;
        float max_value = 0;
        float current_value = 0;

        Surface bar_surface;

        sf::RenderTexture cached_tex;
        sf::Vector2f prev_size;

    public:
        enum DIRECTION
        {
            L_TO_R,
            R_TO_L,
            T_TO_B,
            B_TO_T
        };
    private:
        DIRECTION direction = L_TO_R;

        void drawAction() override;

    public:
        using Gui_element::Gui_element;

        void setDirection(DIRECTION dir);

        void setMinValue(float val);
        float getMinValue() const;
        void setMaxValue(float val);
        float getMaxValue() const;
        void setValue(float val);
        float getValue() const;

        void setBarSurface(const Surface& surf);
    };
}