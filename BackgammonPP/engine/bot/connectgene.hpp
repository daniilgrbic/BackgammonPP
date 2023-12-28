#pragma once

#include <cstddef>

class ConnectGene{
public:
    ConnectGene() = default;
    ConnectGene(ConnectGene* gene) : out(gene->out), into(gene->into), weight(gene->weight), enabled(gene->enabled), innovation(gene->innovation) {}
    ConnectGene(int out, int into, double weight, bool enabled, int innov) : out(out), into(into), weight(weight), enabled(enabled), innovation(innov) {}
    int into;
    int out;
    double weight;
    bool enabled = true;
    size_t innovation;
};
