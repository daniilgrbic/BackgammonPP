#pragma once

#include "innovation.hpp"

class ConnectGene{
public:
    ConnectGene(int into, int out, double weight, bool enabled, int innov) : into(into), out(out), weight(weight), enabled(enabled), innovation(innov) {
    }
    int into;
    int out;
    double weight;
    bool enabled = true;
    Innovation innovation;
};
