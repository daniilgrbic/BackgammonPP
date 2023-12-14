#pragma once

#include "connectgene.hpp"
#include "nodegene.hpp"
#include "network.hpp"
#include "neuron.hpp"
#include "species.hpp"
#include "innovation.hpp"
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
    static double disjoint(Genome& g1, Genome& g2);
    static double weights(Genome& g1, Genome& g2);
    static bool sameSpecies(Genome& g1, Genome& g2);

    Genome(std::string filename);
    Genome();
    Genome(const Genome& g1,const Genome& g2);
    Genome(const Genome& genome);

    void mutateAddConnection();
    void mutateAddNode();
    void mutateConnectionWeight();
    void mutateEnableConnection();
    void mutateDisableConnection();
    void mutate();

    void printToFile(std::string filename);


    QVector<ConnectGene*> genes;
//    QVector<NodeGene> nodegenes;
    int fitness;
    int adjustedFitness;
    Network* network;
    Innovation* innovation;
//    QMap<std::string, double> mutationRates;
    int maxNeuron;
};
