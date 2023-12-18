#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "utility/threadsafequeue.h"
#include "engine/bot/genome.hpp"
#include <random>
#include <cmath>
#include <thread>

namespace AI{
extern size_t inputSize;
extern size_t outputSize;

extern size_t populationSize;
extern size_t generations;
extern double deltaDisjoint;
extern double deltaWeights;
extern double deltaTreshold;

extern size_t staleSpecies;

extern double mutateConnectionsChance;
extern double perturbChance;
extern double crossoverChance;
extern double linkMutationChance;
extern double nodeMutationChance;
extern double biasMutationChance;
extern double disableMutationChance;
extern double enableMutationChance;
extern double stepSize;

extern size_t maxNodes;

extern double sigmoid(const double x);

extern std::default_random_engine generator;
extern std::uniform_real_distribution<double> random01;

typedef std::tuple<Genome&, Genome&, std::atomic<int>&, std::atomic<int>&> Job;

class Neat{
public:
    static void calculateFitness(std::vector<Genome>& population);
    static void savePopulation(std::vector<Genome>& population, std::string filename);
    static void loadPopulation(std::vector<Genome>& population, std::string filename);
};
}
