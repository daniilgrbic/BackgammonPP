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
    Network(QString filename);
    double evaluateNetwork(const QVector<double>& inputs);
private:
    QMap<int, Neuron> neurons;
    QVector<Neuron> sorted_topological;
};
