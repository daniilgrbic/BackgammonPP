#pragma once
#include "genome.hpp"
#include <vector>

class Genome;
class Species{
public:
    static std::vector<Species> createSpecies(std::vector<Genome>& population);
    double calculateAverageFitness();
    void cullSpecies(bool cutToOne);
    void addToSpecies(Genome* g);

    int topFitness;
    int staleness;
    std::vector<Genome*> genomes;
    double averageFitness;

};
