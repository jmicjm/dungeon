#include "scroll.h"

namespace gui
{
    int Scroll::visibleContentLength() const
    {
        return visible_content_length < 0 ? getSize().y : visible_content_length;
    }

    void Scroll::redraw(const sf::Vector2i& size_diff)
    {
        draw(line);
        draw(top_arrow, false);
        draw(bottom_arrow, false);

        const int area_len = getSize().y - top_arrow.getSize().y - bottom_arrow.getSize().y;
        if (area_len > 0)
        {
            const float visible_perc = static_cast<float>(visibleContentLength()) / getContentLength();
            const int handle_len = std::max(1.f, area_len * visible_perc);
            handle.setSizeInfo({ sf::Vector2f( 0, handle_len ), { 100,0 } });

            const float pos_perc = static_cast<float>(getTopPosition()) / (getContentLength() - visibleContentLength());   
            const int handle_pos = top_arrow.getSize().y + (area_len - handle_len) * pos_perc;
            handle.setPositionInfo({ { 0,handle_pos } });

            draw(handle);
        }
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

        top_arrow.setSizeInfo({ {0,16}, {100,0} });
        bottom_arrow.setSizeInfo({ {0,16}, {100,0} });
        bottom_arrow.setPositionInfo({ {0,0}, {0,0}, {0,100} });
        handle.setSizeInfo({ {0,0}, {100,100} });
        line.setSizeInfo({ { 0,16 }, { 100,0 } });
    }

    void Scroll::update()
    {
        top_arrow.update();
        bottom_arrow.update();
        setTopPosition(getTopPosition());
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
        top_position = std::clamp(position, 0, content_length - visibleContentLength());
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