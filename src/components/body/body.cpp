#include "body.h"


Body_attribute_t Body::getAttribute(const entt::registry& registry, Body_attribute attr) const
{
    Body_attribute_t attr_val = 0;

    if (const auto it = base_attributes.attributes.find(attr); it != base_attributes.attributes.end())
    {
        attr_val += it->second;

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
    }

    return attr_val;
}

Body_part_attribute_t Body::getPartsAttribute(const entt::registry& registry, Body_part_attribute attr) const
{
    Body_part_attribute_t attr_val = 0;

    graph.foreach([&](const Body_part& bp) {
        attr_val += bp.getAttribute(registry, attr);
    });

    return attr_val;
}

Body_stat_t Body::getStat(Body_stat stat) const
{
    auto stat_val = 0;

    if (const auto it = stats.stats.find(stat); it != stats.stats.end()) stat_val += it->second;

    return stat_val;
}

Body_part_stat_t Body::getPartsStat(Body_part_stat stat) const
{
    Body_part_stat_t stat_val = 0;

    graph.foreach([&](const Body_part& bp) {
        stat_val += bp.getStat(stat);
        });

    return stat_val;
}

std::string Body::describe(const std::string& pronoun) const
{
    return graph.root ? graph.root->describe(pronoun) : pronoun + " has no body.";
}
