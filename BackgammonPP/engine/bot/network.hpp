#pragma once
#include "genome.hpp"
#include "neuron.hpp"
#include "engine/core/boardstate.h"
#include <fstream>
#include <QVector>
#include <QString>
#include <QMap>

class Genome;
class Network{
public:
    Network(Genome genome);
    Network(std::string filename);
    double evaluateNetwork(const QVector<double>& inputs);
    const QVector<double> inputFromState(PlayerColor player, const BoardState& board);
private:
    void calculateNeuron(Neuron& neuron);
    QMap<int, Neuron> neurons;
};