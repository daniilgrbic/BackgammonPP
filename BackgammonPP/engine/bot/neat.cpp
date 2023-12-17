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

size_t populationSize = 5;
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
    return 2 / (1 + exp(-4.9 * x)) - 1;
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
    int no_workers = 8;
    std::vector<std::atomic<int>> results(AI::populationSize);
    std::vector<std::thread> threads;
    ThreadSafeQueue<Job> jobs;
    std::atomic<bool> done = false;
    for(int i = 0; i < no_workers; i++)
        threads.push_back(std::thread(doWork, std::ref(jobs), std::ref(done)));

    for(int n = 0; n < 3; ++n){
        for(int i = 0; i < AI::populationSize; ++i){
            for(int j = 0; j < AI::populationSize; ++j){
                if(&population[i] != &population[j]){
                    jobs.push(std::make_tuple(std::ref(population[i]), std::ref(population[j]), std::ref(results[i]), std::ref(results[j])));
                }
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

}



/*
newGeneration()
initializePool()
initializeRun()
evaluateCurrent()
nextGenome()
fitnessAlreadyMeasured()
writeFile(filename)
savePool()
loadFile(filename)
loadPool()
playTop()

*/
