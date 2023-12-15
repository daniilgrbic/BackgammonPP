#include "mainwindow.h"
#include "engine/bot/genome.hpp"
#include "engine/bot/network.hpp"
#include "engine/bot/neat.hpp"
#include <QApplication>
#include <QVector>
#include <iostream>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    std::cout << "Starting reading..." << std::endl;
    Genome genome("D:\\backgammon\\BackgammonPP\\engine\\bot\\saved_genomes\\lilith_copy.genome");
    std::cout << "Genome read" << std::endl;
    for(int i = 0; i < 5; ++i){
        genome.mutate();
    }
    Genome genome1(genome);
    Genome genome2(genome);
    for(int i = 0; i < 5; ++i){
        genome1.mutate();
    }
    for(int i = 0; i < 5; ++i){
        genome2.mutate();
    }

    Genome winner = Genome::playBackgammon(genome1, genome2);
    if(&winner == &genome1){
        std::cout << "genome1 Won" << std::endl;
    }else if(&winner == &genome2){
        std::cout << "genome2 Won" << std::endl;
    }
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
