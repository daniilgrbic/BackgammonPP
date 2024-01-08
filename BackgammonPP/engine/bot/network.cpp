#include "network.hpp"
#include "neat.hpp"
#include "genome.hpp"
#include "engine/core/playercolor.h"
#include <QString>
#include <QStack>

Network::Network(Genome genome){
    neurons.resize(genome.maxNeuron);
    for(auto& gene : genome.genes){
        if(!gene.enabled)
            continue;
        neurons[gene.into].incoming.push_back(gene);
    }
}

Network::Network(std::ifstream& filestream){
    Genome genome(filestream);
    delete genome.innovation;
    neurons.resize(genome.maxNeuron);
    for(auto& gene : genome.genes){
        if(!gene.enabled)
            continue;
        neurons[gene.into].incoming.push_back(gene);
    }
}
void Network::calculateNeuron(Neuron& neuron){
    double value = 0.0;
    for(auto& incoming : neuron.incoming){
        if(!neurons[incoming.out].calculated){
            calculateNeuron(neurons[incoming.out]);
        }
        value += neurons[incoming.out].value * incoming.weight;
    }
    neuron.value = AI::sigmoid(value);
    neuron.calculated = true;
}

double Network::evaluateNetwork(const QVector<double>& inputs){
    assert(AI::inputSize - 1 == inputs.size());
    for(int i = 0; i < AI::inputSize - 1; i++){ //Input neurons
        neurons[i].value = inputs[i];
        neurons[i].calculated = true;
    }
    neurons[AI::inputSize - 1].value = 1; //Bias
    neurons[AI::inputSize - 1].calculated = true;

    calculateNeuron(neurons[AI::inputSize]);

    for(auto& neuron : neurons){
        neuron.calculated = false;
    }
    return AI::sigmoid(neurons[AI::inputSize].value);
}

const QVector<double> Network::inputFromState(const BoardState& board){
    PlayerColor opponent = PlayerColor::BLACK;
    PlayerColor player = PlayerColor::WHITE;
    QVector<double> inputs(AI::inputSize - 1);
    for(int i = 0; i < 24; ++i){
        if(board.point(i+1).count() == 0)
            inputs[i] = board.point(i+1).count();
        else{
            if(board.point(i+1).owner() == player){
                inputs[i] = board.point(i+1).count();
            }else{
                inputs[i] = -board.point(i+1).count();
            }
        }
    }
    inputs[24] = board.bar(player);
    inputs[25] = board.bar(opponent);
    inputs[26] = board.off(player);
    inputs[27] = board.off(opponent);

    return inputs;

}
