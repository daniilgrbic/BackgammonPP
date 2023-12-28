#pragma once
#include "genome.hpp"
#include <vector>

class Genome;
class Species{
public:
    double calculateAverageFitness();
    void cullSpecies(bool cutToOne);
    void addToSpecies(Genome* g);
private:
    int topFitness;
    int staleness;
    std::vector<Genome*> genomes;
    int averageFitness;

};
