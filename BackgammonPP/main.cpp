#include "mainwindow.h"
#include "engine/bot/genome.hpp"
#include "engine/bot/network.hpp"
#include "engine/bot/neat.hpp"
#include <QApplication>
#include <QVector>
#include <iostream>
#include <algorithm>
#include <string>

int main(int argc, char *argv[])
{
    std::cout << "Starting reading..." << std::endl;
    Genome genome("../BackgammonPP/engine/bot/saved_genomes/lilith_copy.genome");
    std::cout << "Genome read" << std::endl;


    std::vector<Genome> population(AI::populationSize, genome);
//    AI::Neat::loadPopulation(population, "../BackgammonPP/engine/bot/saved_genomes/gen_70.population");
    std::vector<Genome> newPopulation(AI::populationSize, Genome());
    AI::Neat::calculateFitness(population);
    std::sort(population.begin(), population.end(), [](Genome a, Genome b)
              {
                  return a.fitness > b.fitness;
              });
    for(int i = 0; i < AI::generations; ++i){
        std::cout << "Generation " << i << std::endl;
        for(int j = 0; j < AI::populationSize; ++j){

            std::vector<Genome> out;
            std::sample(population.begin(), population.begin() + AI::populationSize/2, std::back_inserter(out), 2, std::mt19937 {std::random_device{}()});
            newPopulation[j] = Genome(out[0], out[1]);
            newPopulation[j].mutate();
        }
        std::copy(newPopulation.begin(), newPopulation.end(), population.begin());
        std::cout << "calculating fitness for gen " << i << "..." << std::endl;
        AI::Neat::calculateFitness(population);
        std::sort(population.begin(), population.end(), [](Genome a, Genome b)
                  {
                      return a.fitness > b.fitness;
                  });
        for(auto& g : population){
            std::cout << g.fitness << " ";
        }
        std::cout << std::endl;
        AI::Neat::savePopulation(population, std::string("../BackgammonPP/engine/bot/saved_genomes/gen_").append(std::to_string(i)).append(".population"));
    }
    std::cout << population[0].fitness << std::endl;
    population[0].printToFile("../BackgammonPP/engine/bot/saved_genomes/eva.genome");

    return 0;
}
