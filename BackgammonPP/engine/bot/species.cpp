#include "species.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<Species> Species::createSpecies(std::vector<Genome>& population){

    std::vector<Species> species;

    for(int i = 0 ; i < population.size(); ++i) {
        bool foundSpecies = false;
        population[i].adjustedFitness = 1.0;

        for (auto& s : species) {
            if (!s.genomes.empty() && Genome::sameSpecies(population[i], *(s.genomes[0]))) {
                s.addToSpecies(&population[i]);
                foundSpecies = true;
                break;
            }
        }

        if (!foundSpecies) {
            Species newSpecies;
            newSpecies.addToSpecies(&population[i]);
            species.push_back(newSpecies);
        }
    }
    for(auto&s : species){
        s.averageFitness = s.calculateAverageFitness();
    }

    return species;
}

double Species::calculateAverageFitness() {
    double totalFitness = 0.0;
    for (const auto& genome : genomes) {
        totalFitness += genome->fitness;
    }
    averageFitness = totalFitness / genomes.size();
    return averageFitness;
}

void Species::cullSpecies(bool cutToOne) {
    std::sort(genomes.begin(), genomes.end(), [](Genome* a, Genome* b) {
        return a->fitness > b->fitness;
    });
    if (cutToOne && genomes.size() > 1) {
        for (size_t i = 1; i < genomes.size(); ++i) {
            delete genomes[i];
        }
        genomes.erase(genomes.begin() + 1, genomes.end());
    }
}

void Species::addToSpecies(Genome* g) {
    genomes.push_back(g);
}
