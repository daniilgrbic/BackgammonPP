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
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    std::cout << "Starting reading..." << std::endl;
    Genome genome("../BackgammonPP/engine/bot/saved_genomes/lilith_copy.genome");
    std::cout << "Genome read" << std::endl;
//    for(int i = 0; i < 5; ++i){
//        genome.mutate();
//    }
//    Genome genome1(genome);
//    Genome genome2(genome);
//    for(int i = 0; i < 5; ++i){
//        genome1.mutate();
//    }
//    for(int i = 0; i < 5; ++i){
//        genome2.mutate();
//    }


    std::vector<Genome> population(AI::populationSize, genome);
    std::vector<Genome> newPopulation(AI::populationSize, genome);
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
        population[0].printToFile(std::string("../BackgammonPP/engine/bot/saved_genomes/gen_").append(std::to_string(i)).append(".genome"));
    }
    std::cout << population[0].fitness << std::endl;
    population[0].printToFile("../BackgammonPP/engine/bot/saved_genomes/eva.genome");










//    Genome genome3(genome1, genome2);
//    std::cout << genome.innovation->innovation << std::endl;
//    genome.printToFile("D:\\backgammon\\BackgammonPP\\engine\\bot\\saved_genomes\\lilith_mutated.genome");
//    genome1.printToFile("D:\\backgammon\\BackgammonPP\\engine\\bot\\saved_genomes\\lilith_mutated1.genome");
//    genome2.printToFile("D:\\backgammon\\BackgammonPP\\engine\\bot\\saved_genomes\\lilith_mutated2.genome");
//    genome3.printToFile("D:\\backgammon\\BackgammonPP\\engine\\bot\\saved_genomes\\lilith_mutated3.genome");
//    Network net(genome);
//    QVector<double> inputs(28, 1);

//    std::cout << inputs.size() << std::endl;
//    std::cout << net.evaluateNetwork(inputs) << std::endl;

    return 0;
}
