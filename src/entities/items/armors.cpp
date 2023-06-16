#include "armors.h"
#include "../../components/armor.h"
#include "../../components/body/body_part_attributes.h"
#include "../../components/description.h"
#include <vector>
#include <ranges>


using enum Body_part_type;
using enum Body_part_attribute;

static std::vector<const Body_part*> pathBetween(const Body_part& src, const Body_part& dst)
{
    std::vector<const Body_part*> path = { &src };
    std::vector<const Body_part*> came_from = { nullptr };
    std::vector<size_t> next_idx = { 0 };

    const auto push = [&](const Body_part* bp, size_t idx)
    {
        came_from.push_back(path.back());
        path.push_back(bp);
        next_idx.back() = idx;
        next_idx.push_back(0);
    };

    const auto pop = [&]
    {
        came_from.pop_back();
        path.pop_back();
        next_idx.pop_back();
    };

    const auto next = [&]() -> std::pair<const Body_part*, size_t>
    {
        if (next_idx.back() > path.back()->childCount()) return { nullptr, 0 };
        for (auto idx = next_idx.back(); idx < path.back()->childCount(); idx++)
        {
            if (&path.back()->getChild(idx) != came_from.back())
            {
                return { &path.back()->getChild(idx), idx };
            }
        }
        if (path.back()->getParent() && path.back()->getParent() != came_from.back()) return { path.back()->getParent(), path.back()->childCount() };
        return { nullptr, 0 };
    };

    while(!path.empty())
    {
        if (path.back() == &dst) return path;

        const auto [node, idx] = next();
        if (node) push(node, idx+1);
        else pop();
    }

    return {};
}

static bool upperTorsoArmorCoversCheck(const Body_part& equipped_on, const Body_part& other)
{
    if (&equipped_on == &other) return true;

    const auto path = pathBetween(equipped_on, other);

    if (path.size() > 3) return false;

    for(const auto bp : path | std::views::drop(1))
    { 
        if (bp->type != JOINT && bp->type != FOREARM) return false;
    }

    return true;
}

void createFurArmorUpperTorso(entt::registry& registry, const entt::entity item)
{
    registry.emplace<Armor>(item, UPPER_TORSO);
    registry.emplace<Description>(item, "fur armor");

    Body_part_attribute_modifiers ba_modifiers = {
        .modifiers = {
            { 
                DEFENSE,
                {
                    .absolute = 2,
                    .cover_check = upperTorsoArmorCoversCheck
                }
            }
        }
    };

    registry.emplace<Body_part_attribute_modifiers>(item, std::move(ba_modifiers));
}

void createChainmailArmorUpperTorso(entt::registry& registry, const entt::entity item)
{
    registry.emplace<Armor>(item, UPPER_TORSO);
    registry.emplace<Description>(item, "chainmail armor");

    Body_part_attribute_modifiers ba_modifiers = {
        .modifiers = {
            {
                DEFENSE,
                {
                    .absolute = 5,
                    .cover_check = upperTorsoArmorCoversCheck
                }
            }
        }
    };

    registry.emplace<Body_part_attribute_modifiers>(item, std::move(ba_modifiers));
}
