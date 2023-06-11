#pragma once
#include <map>


using Body_stat_t = float;

enum class Body_stat
{
    MANA,
    SATIETY
};

struct Body_stats
{
    using enum Body_stat;

    std::map<Body_stat, Body_stat_t> stats = {
        { MANA, 100 },
        { SATIETY, 100 }
    };
};