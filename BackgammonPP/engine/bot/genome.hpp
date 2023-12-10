#pragma once

#include "gene.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "species.hpp"
#include <string>
#include <map>
#include <vector>
#include <QString>
class Network;
class Species;
class Genome{
public: 
    static Genome crossover(Genome g1, Genome g2);
    static double disjoint(Genome g1, Genome g2);
    static double weights(Genome g1, Genome g2);
    static bool sameSpecies(Genome g1, Genome g2);

    Genome(QString filename);

    Neuron randomNeuron();
    bool containsLink(int out, int into);
    void pointMutate();
    void linkMutate(double forceBias);
    void nodeMutate();
    void enableDisableMutate(bool enable);
    void mutate();
    Genome breedChild(Species species);

    void printToFile(QString filename);


    std::vector<Gene> genes;
    int fitness;
    int adjustedFitness;
    Network* network;
    Neuron maxneuron;
    std::map<std::string, double> mutationRates;

};
