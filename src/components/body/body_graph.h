#pragma once
#include "body_part.h"


struct Body_graph
{
    std::unique_ptr<Body_part> root;

    Body_graph() = default;
    Body_graph(const Body_graph& other) 
    {
        root = other.root ? std::make_unique<Body_part>(*other.root) : nullptr;
    }
    Body_graph(Body_graph&& other) noexcept = default;
    Body_graph& operator=(const Body_graph& other)
    {
        root = other.root ? std::make_unique<Body_part>(*other.root) : nullptr;
        return *this;
    }
    Body_graph& operator=(Body_graph&& other) noexcept = default;
};