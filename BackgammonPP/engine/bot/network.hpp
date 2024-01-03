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
    Network(std::ifstream& filename);
    double evaluateNetwork(const QVector<double>& inputs);
    const QVector<double> inputFromState(const BoardState& board);
private:
    void calculateNeuron(Neuron& neuron);
    std::vector<Neuron> neurons;
};
