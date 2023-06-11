#pragma once
#include <map>

using Body_part_stat_t = float;

enum class Body_part_stat
{
    HP
};

struct Body_part_stats
{
    using enum Body_part_stat;

    std::map<Body_part_stat, Body_part_stat_t> stats = {
        { HP, 100 }
    };
};