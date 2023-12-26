#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "neat.hpp"
#include "utility/threadsafequeue.h"
#include "engine/bot/genome.hpp"
#include <random>
#include <thread>

namespace AI{
size_t inputSize = 29;
size_t outputSize = 1;

size_t populationSize = 100;
size_t generations = 10000;
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



void doWork(ThreadSafeQueue<Job> &jobs, std::atomic<bool> &done){
    while(true){
        std::optional<Job> next = jobs.pop();
        if(next.has_value()){
            auto value = next.value();
            Genome::playBackgammon(std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
        }else{
            if(done && jobs.empty())
                return;
            std::this_thread::yield();
        }

    }
}


void Neat::calculateFitness(std::vector<Genome>& population){
    int no_workers = 16;
    std::vector<std::atomic<int>> results(AI::populationSize);
    std::vector<std::thread> threads;
    ThreadSafeQueue<Job> jobs;
    std::atomic<bool> done = false;
    for(int i = 0; i < no_workers; i++)
        threads.push_back(std::thread(doWork, std::ref(jobs), std::ref(done)));

    for(int n = 0; n < 1; ++n){
        for(int i = 0; i < AI::populationSize; ++i){
            for(int j = i + 1; j < AI::populationSize; ++j){
                    jobs.push(std::make_tuple(std::ref(population[i]), std::ref(population[j]), std::ref(results[i]), std::ref(results[j])));
            }
        }
    }
    done = true;
    for(auto& tr : threads){
        tr.join();
    }
    std::cout << "done \n";
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
            filestream << id++ << " " << gene.out << " " << gene.into << " " << gene.weight <<  " " << gene.enabled << " " << gene.innovation << std::endl;
        }
    }
}
void Neat::loadPopulation(std::vector<Genome>& population, std::string filename){
    std::ifstream filestream(filename);
    int size;
    filestream >> size;
    assert(AI::populationSize == size);
    population.reserve(size);
    int innov;
    for(int i = 0; i < size; ++i){
        int numGenes;
        filestream >> numGenes;
        filestream >> population[i].maxNeuron >> innov;
        int id;
        int into;
        int out;
        double weight;
        bool enabled;
        int innovation_num;
        for(int j = 0; j < numGenes; ++j){
            filestream >> id >> out >> into >> weight >> enabled >> innovation_num;
            population[i].genes.push_back(ConnectGene(out, into, weight, enabled, innovation_num));
        }
    }
    Innovation* innovation = new Innovation(innov);
    for(int i = 0; i < size; ++i){
        population[i].innovation = innovation;
    }
}

}


