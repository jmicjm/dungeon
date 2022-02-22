#include "visibleAreaBounds.h"
#include "../../utils/sf_vector2_utils.h"

std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBoundsTiles(const sf::View& view, const sf::Vector2i& tile_size)
{
    const auto [tl_px, br_px] = visibleAreaBoundsPixels(view);
    const sf::Vector2i tl_tile{ vecFloor(vecDiv(sf::Vector2f{ tl_px }, tile_size)) };
    const sf::Vector2i br_tile{ vecCeil(vecDiv(sf::Vector2f{ br_px }, tile_size)) };

    return { tl_tile, br_tile };
}

std::pair<sf::Vector2i, sf::Vector2i> visibleAreaBoundsPixels(const sf::View& view)
{
    const sf::FloatRect view_rect = { view.getCenter() - view.getSize() / 2.f, view.getSize() };
    const sf::FloatRect bounding_box = sf::Transform{}.rotate(view.getRotation(), view.getCenter()).transformRect(view_rect);

    const sf::Vector2i tl_px{ vecFloor(sf::Vector2f{ bounding_box.left, bounding_box.top }) };
    const sf::Vector2i br_px{ vecCeil(sf::Vector2f{ tl_px } + sf::Vector2f{ bounding_box.width, bounding_box.height }) };

    return { tl_px, br_px };
}
