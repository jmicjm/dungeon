#pragma once
#include "body_part.fwd.h";
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

    using covers_f = bool(const Body_part& equipped_on, const Body_part& other);
    covers_f* cover_check = [](const Body_part& equipped_on, const Body_part& other) { return &equipped_on == &other; };

    bool covers(const Body_part& equipped_on, const Body_part& other) const
    {
        return cover_check ? cover_check(equipped_on, other) : false;
    }
};

struct Body_part_attribute_modifiers
{
    std::map<Body_part_attribute, Body_part_attribute_modifier> modifiers;
};