#include "body.h"


using enum Body_stat;
using enum Body_attribute;
using enum Body_part_attribute;

std::optional<Body_attribute_t> Body::getAttribute(const entt::registry& registry, Body_attribute attr) const
{
    if (const auto it = base_attributes.attributes.find(attr); it != base_attributes.attributes.end())
    {
        Body_attribute_t attr_val = it->second;

        graph.foreach([&](const Body_part& bp) {
            if (const auto inv = bp.getInventory(registry))
            {
                Body_attribute_modifier modifier;

                inv->foreach([&](auto entity) {
                    if (const auto modifiers = registry.try_get<Body_attribute_modifiers>(entity))
                    {
                        if (const auto it = modifiers->modifiers.find(attr); it != modifiers->modifiers.end())
                        {
                            modifier.absolute += it->second.absolute;
                            modifier.percentage += it->second.percentage;
                        }
                    }
                });

                attr_val += modifier.absolute;
                attr_val = attr_val * (1 + modifier.percentage);
            }
        }); 

        return attr_val;
    }

    return {};
}

std::optional<Body_part_attribute_t> Body::getPartsAttribute(const entt::registry& registry, Body_part_attribute attr) const
{
    Body_part_attribute_t attr_val = 0;
    bool have_attr = false;

    graph.foreach([&](const Body_part& bp) {
        if (const auto bpattr = bp.getAttribute(registry, attr))
        {
            attr_val += *bpattr;
            have_attr = true;
        }
    });

    return have_attr ? std::optional{ attr_val } : std::optional<Body_part_stat_t>{};
}

std::optional<Body_stat_t> Body::getStat(Body_stat stat) const
{
    if (const auto it = stats.stats.find(stat); it != stats.stats.end()) return it->second;
    return {};
}

std::optional<Body_part_stat_t> Body::getPartsStat(Body_part_stat stat) const
{
    Body_part_stat_t stat_val = 0;
    bool have_stat = false;

    graph.foreach([&](const Body_part& bp) {
        if (const auto bpstat = bp.getStat(stat))
        {
            stat_val += *bpstat;
            have_stat = true;
        }
        
    });

    return have_stat ? std::optional{ stat_val } : std::optional<Body_part_stat_t>{};
}

void Body::updateStats(const entt::registry& registry)
{
    graph.foreach([&](Body_part& bp) {
        bp.updateStats(registry);
    });

    if (const auto mana = getStat(MANA))
    {
        stats.stats[MANA] = std::clamp(*mana + getAttribute(registry, MANA_REGEN).value_or(0), 0.f, getAttribute(registry, MAX_MANA).value_or(0));
    }

    if (const auto satiety = getStat(SATIETY))
    {
        stats.stats[SATIETY] = std::clamp(*satiety - getPartsAttribute(registry, SATIETY_USE).value_or(0), 0.f, getAttribute(registry, MAX_SATIETY).value_or(0));
    }
}

void Body::clampStats(const entt::registry& registry)
{
    graph.foreach([&](Body_part& bp) {
        bp.clampStats(registry);
    });

    if (const auto mana = getStat(MANA))
    {
        stats.stats[MANA] = std::clamp(*mana, 0.f, getAttribute(registry, MAX_MANA).value_or(0));
    }

    if (const auto satiety = getStat(SATIETY))
    {
        stats.stats[SATIETY] = std::clamp(*satiety, 0.f, getAttribute(registry, MAX_SATIETY).value_or(0));
    }
}

std::string Body::describe(const std::string& pronoun) const
{
    return graph.root ? graph.root->describe(pronoun) : pronoun + " has no body.";
}
