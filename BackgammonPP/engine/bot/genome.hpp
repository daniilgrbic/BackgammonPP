#pragma once

#include "connectgene.hpp"
#include "nodegene.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "species.hpp"
#include <string>
#include <map>
#include <vector>
#include <QVector>
#include <QString>
#include <QMap>
#include <random>

class Network;
class Species;
class Genome{
public: 
    static Genome crossover(Genome g1, Genome g2);
    static double disjoint(Genome g1, Genome g2);
    static double weights(Genome g1, Genome g2);
    static bool sameSpecies(Genome g1, Genome g2);

    Genome(std::string filename);

    void mutateAddConnection();
    void mutateAddNode();
    void mutateConnectionWeight();
    void mutateEnableConnection();
    void mutateDisableConnection();
    void mutate();

    void printToFile(std::string filename);


    QVector<ConnectGene*> genes;
    QVector<NodeGene> nodegenes;
    int fitness;
    int adjustedFitness;
    int maxNeuron = 0;
    Network* network;
    QMap<std::string, double> mutationRates;
};
