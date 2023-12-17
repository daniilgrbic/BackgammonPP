#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "neat.hpp"
#include "engine/bot/genome.hpp"
#include <random>
#include <thread>

namespace AI{
size_t inputSize = 29;
size_t outputSize = 1;

size_t populationSize = 10;
size_t generations = 1000;
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
double stepSize = 0.10;

size_t maxNodes = 1000000;

double sigmoid(const double x){
    return 1 / (1 + exp(-x));
}

std::default_random_engine generator;
std::uniform_real_distribution<double> random01(0.0, 1.0);


void Neat::calculateFitness(std::vector<Genome>& population){
    std::vector<std::atomic<int>> results(AI::populationSize);
    for(int n = 0; n < 5; ++n){
        for(int i = 0; i < AI::populationSize; ++i){
            std::vector<std::thread> threads;
            for(int j = i + 1; j < AI::populationSize; ++j){
                threads.push_back(std::thread(Genome::playBackgammon,std::ref(population[i]), std::ref(population[j]), std::ref(results[i]), std::ref(results[j])));
            }
            for(auto& tr : threads){
                tr.join();
            }
        }
    }
    for(int i = 0; i < AI::populationSize; ++i){
        population[i].fitness = results[i];
    }
}

void Neat::savePopulation(std::vector<Genome>& population, std::string filename){
    std::ofstream filestream(filename);
    filestream << population.size() << std::endl;
    for(auto& genome : population){
        filestream << genome.genes.size() << std::endl;
        filestream << genome.maxNeuron << " " << genome.innovation->innovation << std::endl;
        int id = 1;
        for(auto gene : genome.genes){
            filestream << id++ << " " << gene->out << " " << gene->into << " " << gene->weight <<  " " << gene->enabled << " " << gene->innovation << std::endl;
        }
    }
}
void Neat::loadPopulation(std::vector<Genome>& population, std::string filename){
    std::ifstream filestream(filename);
    int size;
    filestream >> size;
    assert(AI::populationSize == size);
    population.reserve(size);
    for(int i = 0; i < size; ++i){
        int numGenes;
        filestream >> numGenes;
        int innov;
        filestream >> population[i].maxNeuron >> innov;
        population[i].innovation = new Innovation(innov);
        int id;
        int into;
        int out;
        double weight;
        bool enabled;
        int innovation_num;
        for(int j = 0; j < numGenes; ++j){
            filestream >> id >> out >> into >> weight >> enabled >> innovation_num;
            population[i].genes.push_back(new ConnectGene(out, into, weight, enabled, innovation_num));
        }
    }
}

}


