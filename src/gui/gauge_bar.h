#pragma once
#include "gui_element.h"
#include "gauge_bar_appearance.h"
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

        void redraw() override;
        bool redraw_required = true;

    public:
        using Gui_element::Gui_element;

        bool isRedrawRequired() const override;

        void setDirection(DIRECTION dir);

        void  setMinValue(float val);
        float getMinValue() const;
        void  setMaxValue(float val);
        float getMaxValue() const;
        void  setValue(float val);
        float getValue() const;

        void setAppearance(const Gauge_bar_appearance& a);
        Gauge_bar_appearance getAppearance() const;
    };
}