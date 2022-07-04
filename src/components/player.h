#pragma once
#include <chrono>


struct Player
{
    std::chrono::steady_clock::time_point last_action_t = std::chrono::steady_clock::now();
};
