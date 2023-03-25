#include "list.h"
#include <algorithm>


void gui::List::redraw()
{
    auto it = std::ranges::lower_bound(
        entries,
        0,
        {},
        [](const auto& el) { return el->position().y + el->size().y; }
    );

    while (it != entries.end() && (*it)->position().y <= size().y)
    {
        (*it++)->draw();
    }
    if (scroll.isNeeded()) scroll.draw();
}

void gui::List::resizeEvent(sf::Vector2f size_diff)
{
    updatePositions();
}

void gui::List::activateEvent()
{
    scroll.activate();
    for (auto& entry : entries) entry->activate();
}

void gui::List::deactivateEvent()
{
    scroll.deactivate();
    for (auto& entry : entries) entry->deactivate();
}

void gui::List::updatePositions()
{
    auto current_offset = 0.f;
    for (auto& entry : entries)
    {
        entry->setPositionFunction([&, current_offset](auto vec) {
            return sf::Vector2f{ 0, current_offset - scroll.getTopPosition() };
        });
        current_offset += entry->size().y;
    }
    scroll.setContentLength(current_offset);
}

gui::List::List() 
{
    entries_area.setParent(this);
    entries_area.setSizeFunction([&](auto parent_size) {
        const auto width = scroll.isNeeded() ? parent_size.x - scroll.size().x : parent_size.x;
        return sf::Vector2f{ width, parent_size.y };
    });
    scroll.setParent(this);
    scroll.setPositionInfo({ .relative_to = {1,0} });
    setAppearance(List_appearance{});
}

void gui::List::setEntries(std::vector<std::unique_ptr<Gui_component>> entries)
{
    this->entries = std::move(entries);
    for (auto& entry : this->entries) entry->setParent(&entries_area);
    updatePositions();
}

void gui::List::update()
{
    scroll.update();
    for (auto& entry : entries) entry->update();
}

size_t gui::List::entryCount() const
{
    return entries.size();
}

float gui::List::length() const
{
    return entries.empty() ? 0 : scroll.getContentLength();
}

void gui::List::setAppearance(const List_appearance& a)
{
    scroll.setAppearance(a.scroll_apearance);
    scroll.setSizeInfo({ .fixed = {16,0}, .percentage = {0,1} });
}

gui::List_appearance gui::List::getAppearance() const
{
    return List_appearance{ scroll.getAppearance(), scroll.getSizeInfo().fixed.x };
}
