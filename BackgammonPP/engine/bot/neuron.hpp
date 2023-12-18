#pragma once

#include <vector>
#include "connectgene.hpp"
class Neuron{

public:
    bool calculated = false;
    std::vector<ConnectGene> incoming;
    double value;
};
