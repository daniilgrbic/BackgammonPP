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
#include <atomic>

class Network;
class Species;
class Genome{
public: 
    static double disjoint(const Genome& g1, const Genome& g2);
    static double weights(const Genome& g1, const Genome& g2);
    static bool sameSpecies(const Genome& g1, const Genome& g2);
    static void playBackgammon(Genome& g1, Genome& g2, std::atomic<int>& fit1, std::atomic<int>& fit2);

    Genome() = default;
    Genome(std::ifstream& filestream);
    Genome(const Genome& g1,const Genome& g2);
    Genome(const Genome& genome);

    void mutateAddConnection();
    void mutateAddNode();
    void mutateConnectionWeight();
    void mutateEnableConnection();
    void mutateDisableConnection();
    void mutate();

    void printToFile(std::string filename);


    QVector<ConnectGene> genes;

    int fitness{0};
    int adjustedFitness;
    Network* network;
    Innovation* innovation;
    int maxNeuron;
// maybe will need idk
//    QMap<std::string, double> mutationRates;
//    QVector<NodeGene> nodegenes;

};
