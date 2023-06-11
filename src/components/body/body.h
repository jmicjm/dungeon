#pragma once
#include "body_graph.h"
#include "body_stats.h"
#include "body_attributes.h"
#include <string>
#include <optional>


struct Body
{
    Body_graph graph;

    Body_stats stats;
    Body_base_attributes base_attributes;


    std::optional<Body_attribute_t> getAttribute(const entt::registry& registry, Body_attribute attr) const;
    std::optional<Body_part_attribute_t> getPartsAttribute(const entt::registry& registry, Body_part_attribute attr) const;

    std::optional<Body_stat_t> getStat(Body_stat stat) const;
    std::optional<Body_part_stat_t> getPartsStat(Body_part_stat stat) const;

    void updateStats(const entt::registry& registry);
    void clampStats(const entt::registry& registry);

    std::string describe(const std::string& pronoun) const;
};