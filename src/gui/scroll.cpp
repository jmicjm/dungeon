#include "scroll.h"
#include "../input/input.h"


namespace gui
{
    void Scroll::updateHandle()
    {
        const int area_len = getSize().y - top_arrow.getSize().y - bottom_arrow.getSize().y;

        const float visible_perc = getContentLength() > 0 ? static_cast<float>(visibleContentLength()) / getContentLength() : 1;
        const int handle_len = std::clamp(static_cast<int>(area_len * visible_perc), 1, std::max(1, area_len));
        handle.setSizeInfo({ sf::Vector2f(0, handle_len), { 1,0 } });

        const float pos_perc = static_cast<float>(getTopPosition()) / (getContentLength() - visibleContentLength());
        const int handle_pos = top_arrow.getSize().y + (area_len - handle_len) * pos_perc;
        handle.setPositionInfo({ { 0,handle_pos } });
    }

    int Scroll::visibleContentLength() const
    {
        return visible_content_length < 0 ? getSize().y : visible_content_length;
    }

    void Scroll::redraw(const sf::Vector2i& size_diff)
    {
        draw(line);
        draw(top_arrow, false);
        draw(bottom_arrow, false);
        draw(handle);
    }

    void Scroll::resizeEvent()
    {
        setTopPosition(getTopPosition());
        updateHandle();
    }

    Scroll::Scroll(sf::RenderWindow& rw) 
        : Gui_element(rw), top_arrow(rw), bottom_arrow(rw), handle(rw), line(rw)
    {
        top_arrow.setParent(this);
        top_arrow.setPressFunction([&]() {up(10); });
        top_arrow.setPressDelay(std::chrono::milliseconds(0));
        bottom_arrow.setParent(this);
        bottom_arrow.setPressFunction([&]() {down(10); });
        bottom_arrow.setPressDelay(std::chrono::milliseconds(0));
        handle.setParent(this);
        line.setParent(this);

        top_arrow.setSizeInfo({ {0,16}, {1,0} });
        bottom_arrow.setSizeInfo({ {0,16}, {1,0} });
        bottom_arrow.setPositionInfo({ {0,0}, {0,0}, {0,1} });
        line.setSizeInfo({ { 0,16 }, { 1,0 } });
    }

    void Scroll::update()
    {
        if (!is_holding_handle)
        {
            top_arrow.update();
            bottom_arrow.update();
        }

        updateHandle();

        auto hold_vec = Input::getMouseHoldVec(sf::Mouse::Left);
        if (hold_vec)
        {
            if (is_holding_handle && hold_vec->source_pos != hold_source_pos)
            {
                is_holding_handle = false;
            }

            if (!is_holding_handle 
                && hold_vec->source_pos.x >= handle.getGlobalPosition().x && hold_vec->source_pos.x <= handle.getGlobalPosition().x + handle.getSize().x
                && hold_vec->source_pos.y >= handle.getGlobalPosition().y && hold_vec->source_pos.y <= handle.getGlobalPosition().y + handle.getSize().y)
            {
                is_holding_handle = true;
                hold_source_pos = hold_vec->source_pos;
                hold_top_pos = getTopPosition();
            }
                 
            if (is_holding_handle)
            {      
                const int area_len = getSize().y - top_arrow.getSize().y - bottom_arrow.getSize().y - handle.getSize().y;
                const int shift = hold_vec->current_pos.y - hold_vec->source_pos.y;
                const float perc_shift = static_cast<float>(shift) / area_len;

                setTopPosition(hold_top_pos + (getContentLength() - visibleContentLength()) * perc_shift);
            }
        }
        else
        {
            is_holding_handle = false;
        }

    }

    void Scroll::setContentLength(int length)
    {
        content_length = length;
        setTopPosition(getTopPosition());
    }

    int Scroll::getContentLength() const
    {
        return content_length;
    }

    void Scroll::setVisibleContentLength(int length)
    {
        visible_content_length = length;
    }

    int Scroll::getVisibleContentLength() const
    {
        return visible_content_length;
    }

    void Scroll::setTopPosition(int position)
    {
        top_position = std::clamp(position, 0, std::max(0,content_length - visibleContentLength()));
    }

    int Scroll::getTopPosition() const
    {
        return top_position;
    }

    void Scroll::up(int n)
    {
        setTopPosition(top_position - n);
    }

    void Scroll::down(int n)
    {
        setTopPosition(top_position + n);
    }

    void Scroll::setTopArrowAppearance(Surface pressed, Surface released, Surface hovered)
    {
        top_arrow.setPressedSurface(pressed);
        top_arrow.setReleasedSurface(released);
        top_arrow.setPressedHoveredOverlay(hovered);
        top_arrow.setReleasedHoveredOverlay(hovered);
    }

    void Scroll::setBottomArrowAppearance(Surface pressed, Surface released, Surface hovered)
    {
        bottom_arrow.setPressedSurface(pressed);
        bottom_arrow.setReleasedSurface(released);
        bottom_arrow.setPressedHoveredOverlay(hovered);
        bottom_arrow.setReleasedHoveredOverlay(hovered);
    }

    void Scroll::setHandleSurface(Surface s)
    {
        handle.setBackgroundSurface(s);
    }

    void Scroll::setLineSurface(Surface s)
    {
        line.setBackgroundSurface(s);
    }
}