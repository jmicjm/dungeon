#pragma once
#include <map>


using Body_attribute_t = float;

enum class Body_attribute
{
    MAX_MANA,
    MANA_REGEN,
    MAX_SATIETY,
    STRENGTH,
    DEXTERITY
};

struct Body_base_attributes
{
    using enum Body_attribute;

    std::map<Body_attribute, Body_attribute_t> attributes = {
        { MAX_MANA, 100 },
        { MANA_REGEN, 1 },
        { MAX_SATIETY, 100 },
        { STRENGTH, 1 },
        { DEXTERITY, 1 }
    };
};

struct Body_attribute_modifier
{
    Body_attribute_t absolute = 0;
    Body_attribute_t percentage = 0;
};

struct Body_attribute_modifiers
{
    std::map<Body_attribute, Body_attribute_modifier> modifiers;
};