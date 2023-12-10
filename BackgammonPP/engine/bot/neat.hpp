#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace AI{
size_t inputSize = 29;
size_t outputSize = 1;

size_t populationSize = 300;
double deltaDisjoint = 2.0;
double deltaWeights = 0.4;
double deltaTreshold = 1.0;

size_t staleSpecies = 15;

double mutateConnectionsChance = 0.25;
double perturbChance = 0.90;
double crossoverChance = 0.75;
double linkMutationChance = 2.0;
double nodeMutationChance = 0.50;
double biasMutationChance = 0.40;
double disableMutationChance = 0.40;
double enableMutationChance = 0.20;

size_t maxNodes = 1000000;

double sigmoid(const double x){
    return 2 / (1 + exp(-4.9 * x)) - 1;
}

}
