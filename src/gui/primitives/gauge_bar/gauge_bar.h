#pragma once
#include "../../gui_component.h"
#include "../../surface.h"
#include "gauge_bar_appearance.h"
#include "../../../gfx/copyable_rendertexture.h"

namespace gui
{
    class Gauge_bar : public Gui_component
    {
        float min_value = 0;
        float max_value = 0;
        float current_value = 0;

        Surface bar_surface;

        Copyable_rendertexture rtex;
        bool render_required = true;
        void renderBar();

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
        void resizeEvent(sf::Vector2f size_diff) override;

    public:
        Gauge_bar();

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