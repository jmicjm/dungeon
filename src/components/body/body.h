#pragma once
#include "body_graph.h"
#include "body_stats.h"
#include "body_attributes.h"
#include <string>


struct Body
{
    Body_graph graph;

    Body_stats stats;
    Body_base_attributes base_attributes;


    Body_attribute_t getAttribute(const entt::registry& registry, Body_attribute attr) const;
    Body_part_attribute_t getPartsAttribute(const entt::registry& registry, Body_part_attribute attr) const;

    Body_stat_t getStat(Body_stat stat) const;
    Body_part_stat_t getPartsStat(Body_part_stat stat) const;


    std::string describe(const std::string& pronoun) const;
};