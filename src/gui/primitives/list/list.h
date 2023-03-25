#pragma once
#include "../../gui_component.h"
#include "list_appearance.h"
#include "../scroll/scroll.h"
#include <vector>
#include <memory>


namespace gui
{
    class List : public Gui_component
    {
        std::vector<std::unique_ptr<Gui_component>> entries;
        Gui_component entries_area;
        Scroll scroll;

        void redraw() override;

        void resizeEvent(sf::Vector2f size_diff) override;
        void activateEvent() override;
        void deactivateEvent() override;

        void updatePositions();

    public:
        List();

        void setEntries(std::vector<std::unique_ptr<Gui_component>> entries);

        void update() override;

        size_t entryCount() const;
        float length() const;

        void setAppearance(const List_appearance& a);
        List_appearance getAppearance() const;
    };
}