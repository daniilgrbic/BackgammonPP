#pragma once
#include "genome.hpp"
#include "neuron.hpp"
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
private:
    void calculateNeuron(Neuron& neuron);
    QMap<int, Neuron> neurons;
};
