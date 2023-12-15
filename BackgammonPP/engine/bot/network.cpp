#include "network.hpp"
#include "neat.hpp"
#include "genome.hpp"
#include "engine/core/playercolor.h"
#include <QString>
#include <QStack>

Network::Network(Genome genome){
    for(auto gene : genome.genes){
        if(!gene->enabled)
            continue;
        if(neurons.find(gene->into) != neurons.end()){
            neurons[gene->into].incoming.push_back(gene);
        }else{
            neurons.insert(gene->into, Neuron());
            neurons[gene->into].incoming.push_back(gene);
        }
        if(neurons.find(gene->out) == neurons.end()){
            neurons.insert(gene->out, Neuron());
        }
    }
}

Network::Network(std::string filename){
    Genome genome(filename);
    for(auto gene : genome.genes){
        if(!gene->enabled)
            continue;
        if(neurons.find(gene->into) != neurons.end()){
            neurons[gene->into].incoming.push_back(gene);
        }else{
            neurons.insert(gene->into, Neuron());
            neurons[gene->into].incoming.push_back(gene);
        }
        if(neurons.find(gene->out) == neurons.end()){
            neurons.insert(gene->out, Neuron());
        }
    }
}
void Network::calculateNeuron(Neuron& neuron){
    double value = 0.0;
    for(auto incoming : neuron.incoming){
        if(!neurons[incoming->out].calculated){
            calculateNeuron(neurons[incoming->out]);
        }
        value += neurons[incoming->out].value * incoming->weight;
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
    for(auto neuron : neurons){
        neuron.calculated = false;
    }
    return neurons[AI::inputSize].value;
}

const QVector<double> Network::inputFromState(PlayerColor player, const BoardState& board){
    PlayerColor opponent = PlayerColor::BLACK;
    BoardState currBoard(board);
    if(PlayerColor::BLACK == player){
        currBoard.mirror();
        opponent = PlayerColor::WHITE;
    }
    QVector<double> inputs(AI::inputSize - 1);
    for(int i = 0; i < 24; ++i){
        if(board.point(i).count() == 0)
            inputs[i] = board.point(i).count();
        else{
            if(board.point(i).owner() == player){
                inputs[i] = board.point(i).count();
            }else{
                inputs[i] = -board.point(i).count();
            }
        }
    }
    inputs[24] = currBoard.bar(player);
    inputs[25] = currBoard.bar(opponent);
    inputs[26] = currBoard.off(player);
    inputs[27] = currBoard.off(opponent);

    return inputs;

}
