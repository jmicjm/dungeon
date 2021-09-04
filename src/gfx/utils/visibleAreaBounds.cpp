#include "visibleAreaBounds.h"
#include "../../utils/sf_vector2_utils.h"

std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBounds(const sf::View& view, const sf::Vector2i& tile_size)
{
    const auto [tl_px, br_px] = visibleAreaBounds(view);
    const sf::Vector2i tl_tile = vecDiv(sf::Vector2i{ tl_px }, tile_size);
    const sf::Vector2i br_tile = vecDiv(sf::Vector2i{ br_px }, tile_size);

    return { tl_tile, br_tile };
}

std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBounds(const sf::View& view)
{
    const sf::FloatRect view_rect = { view.getCenter() - view.getSize() / 2.f, view.getSize() };
    const sf::FloatRect bounding_box = sf::Transform{}.rotate(view.getRotation(), view.getCenter()).transformRect(view_rect);

    const sf::Vector2i tl_px = { static_cast<int>(bounding_box.left), static_cast<int>(bounding_box.top) };
    const sf::Vector2i br_px = tl_px + sf::Vector2i{ static_cast<int>(bounding_box.width), static_cast<int>(bounding_box.height) };

    return { tl_px, br_px };
}
