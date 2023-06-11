#pragma once
#include <map>


using Body_part_attribute_t = float;

enum class Body_part_attribute
{
    MAX_HP,
    HP_REGEN,
    SATIETY_USE,
    DEFENSE
};

struct Body_part_base_attributes
{
    using enum Body_part_attribute;

    std::map<Body_part_attribute, Body_part_attribute_t> attributes = {
        { MAX_HP, 100 },
        { HP_REGEN, 1 },
        { SATIETY_USE, 0.01 },
        { DEFENSE, 1}
    };
};

struct Body_part_attribute_modifier
{
    Body_part_attribute_t absolute = 0;
    Body_part_attribute_t percentage = 0;
};

struct Body_part_attribute_modifiers
{
    std::map<Body_part_attribute, Body_part_attribute_modifier> modifiers;
};