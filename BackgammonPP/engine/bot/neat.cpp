#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "neat.hpp"
#include "utility/threadsafequeue.h"
#include "engine/bot/genome.hpp"
#include <random>
#include <thread>
#include <QProgressBar>
#include <QApplication>
#include "progresssignaler.hpp"
#include <sstream>
#include "species.hpp"

namespace AI{
size_t inputSize = 29;
size_t outputSize = 1;
size_t numThreads = 8;
std::string popPath = "";

size_t populationSize = 30;
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
            emit std::get<4>(value)->updateLocalProgress();
        }else{
            if(done && jobs.empty())
                return;
            std::this_thread::yield();
        }

    }
}


void Neat::calculateFitness(std::vector<Genome>& population, ProgressSignaler *ps){
    std::vector<std::atomic<int>> results(AI::populationSize);
    std::vector<std::thread> threads;
    ThreadSafeQueue<Job> jobs;
    std::atomic<bool> done = false;
    for(int i = 0; i < numThreads; i++)
        threads.push_back(std::thread(doWork, std::ref(jobs), std::ref(done)));
    int numRounds=1;
    emit ps->updateLocalRange(0, numRounds*AI::populationSize*(AI::populationSize - 1)/2 - 1);
    for(int n = 0; n < numRounds; ++n){
        for(int i = 0; i < AI::populationSize; ++i){
            for(int j = i + 1; j < AI::populationSize; ++j){
                    jobs.push(std::make_tuple(std::ref(population[i]), std::ref(population[j]), std::ref(results[i]), std::ref(results[j]),ps));
            }
        }
    }
    done = true;
    for(auto& tr : threads){
        tr.join();
    }
    std::stringstream ss;
    ss << "done calculating fitness" << std::endl;
    emit ps->sendText(ss.str().c_str());
    ss.str("");
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

void Neat::train(int generations, int cores, ProgressSignaler *ps){
    std::stringstream ss;
//    Genome genome1("../BackgammonPP/engine/bot/saved_genomes/lilith_copy.genome");
//    Genome genome2("../BackgammonPP/engine/bot/saved_genomes/gen_99.genome");
//    std::atomic<int> won1;
//    std::atomic<int> won2;
//    for(int i = 0; i < 100; ++i){
//        Genome::playBackgammon(genome1, genome2, won1, won2);
//        ss << "game: " << i;
//        emit ps->sendText(ss.str().c_str());
//        ss.str("");
//    }
//    ss << "won1: " << won1 << std::endl;
//    ss << "won2: " << won2 << std::endl;
//    emit ps->sendText(ss.str().c_str());
//    ss.str("");
//    emit ps->done();
//    return;
    AI::numThreads = cores;
    AI::generations = generations;


    ss << "generations: " << AI::generations << std::endl << "threads: " << AI::numThreads << std::endl;
    emit ps->sendText(ss.str().c_str());
    ss.str("");
    ss << "Starting reading..." << std::endl;
    emit ps->sendText(ss.str().c_str());
    ss.str("");

    Genome genome("../BackgammonPP/engine/bot/saved_genomes/lilith_copy.genome");

    ss << "Genome read" << std::endl;
    emit ps->sendText(ss.str().c_str());
    ss.str("");
    int start = 0;
    emit ps->updateGlobalProgress(0);


    std::vector<Genome> population(AI::populationSize, genome);
    if(AI::popPath != ""){
        ss << "Loading population from file..." << std::endl;
        emit ps->sendText(ss.str().c_str());
        ss.str("");
        AI::Neat::loadPopulation(population, AI::popPath);
    }


    //    AI::Neat::loadPopulation(population, std::string("../BackgammonPP/engine/bot/saved_genomes/gen_").append(std::to_string(start)).append(".population"));
    std::vector<Genome> newPopulation(AI::populationSize, Genome());
    AI::Neat::calculateFitness(population, ps);


    std::vector<Species> species = Species::createSpecies(population);
    for(Species& specie : species){
        for(Genome* g : specie.genomes){
            g->adjustedFitness = specie.averageFitness;
        }
    }

    std::sort(population.begin(), population.end(), [](Genome a, Genome b)
              {
                  return a.adjustedFitness > b.adjustedFitness;
              });
    emit ps->updateGlobalProgress(1);

    for(int i = start + 1; i < AI::generations; ++i){
        ss << "Generation " << i << std::endl;
        emit ps->sendText(ss.str().c_str());
        ss.str("");

        for(int j = 0; j < AI::populationSize; ++j){

            std::vector<Genome> out;
            std::sample(population.begin(), population.begin() + AI::populationSize/2, std::back_inserter(out), 2, std::mt19937 {std::random_device{}()});
            if(AI::random01(AI::generator) < AI::crossoverChance){
                newPopulation[j] = Genome(out[0], out[1]);
            }else{
                newPopulation[j] = Genome(out[0]);
            }
            newPopulation[j].mutate();
        }
        std::copy(newPopulation.begin(), newPopulation.end(), population.begin());

        ss << "calculating fitness for gen " << i << "..." << std::endl;
        emit ps->sendText(ss.str().c_str());
        ss.str("");
        AI::Neat::calculateFitness(population, ps);

        std::vector<Species> newSpecies = Species::createSpecies(population);
        for(Species& specie : newSpecies){
            for(Genome* g : specie.genomes){
                g->adjustedFitness = specie.averageFitness;
            }
        }

        std::sort(population.begin(), population.end(), [](Genome a, Genome b)
                  {
                      return a.adjustedFitness > b.adjustedFitness;
                  });

        for(auto& g : population){
            ss << g.fitness << " " << g.adjustedFitness << " ";
            emit ps->sendText(ss.str().c_str());
            ss.str("");
        }

        ss << std::endl;
        emit ps->sendText(ss.str().c_str());
        ss.str("");

        AI::Neat::savePopulation(population, std::string("../BackgammonPP/engine/bot/saved_genomes/gen_").append(std::to_string(i)).append(".population"));
        population[0].printToFile(std::string("../BackgammonPP/engine/bot/saved_genomes/gen_").append(std::to_string(i)).append(".genome"));
        emit ps->updateGlobalProgress(i+1);

    }
    population[0].printToFile("../BackgammonPP/engine/bot/saved_genomes/eva.genome");
    emit ps->done();
}

}


