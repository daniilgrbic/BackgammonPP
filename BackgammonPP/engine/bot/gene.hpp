#pragma once

#include "innovation.hpp"

class Gene{
public:
    int into;
    int out;
    double weight;
    bool enabled = true;
    Innovation innovation;
};
