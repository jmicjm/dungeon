#pragma once
#include "body_graph.h"
#include <string>

struct Body
{
    Body_graph graph;

    std::string describe(const std::string& pronoun) const
    {
        return graph.root ? graph.root->describe(pronoun) : pronoun + " has no body.";
    }
};