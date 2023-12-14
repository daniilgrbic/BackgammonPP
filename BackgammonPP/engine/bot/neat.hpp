#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>

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


}
