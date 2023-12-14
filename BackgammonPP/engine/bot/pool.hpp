#pragma once
#include <vector>
#include "species.hpp"
#include "innovation.hpp"
#include "genome.hpp"

class Pool{
public:
    int rankGlobally();
    double totalAverageFitness();
    void removeStaleSpecies();
    void removeWeakSpecies();
private:
    std::vector<Species> species;
    int generation;
    Innovation innovation;
    Species currentSpecies;
    Genome currentGenome;
    int maxFitness;
};