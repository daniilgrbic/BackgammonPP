#include "network.hpp"
#include "neat.hpp"
#include "genome.hpp"
#include <QString>

Network::Network(Genome genome){
    for(auto gene : genome.genes){
        if(!gene.enabled)
            continue;
        if(neurons.find(gene.into) != neurons.end()){
            neurons[gene.into].incoming.push_back(gene);
        }else{
            neurons.insert(gene.into, Neuron());
            neurons[gene.into].incoming.push_back(gene);
        }
        if(neurons.find(gene.out) == neurons.end()){
            neurons.insert(gene.out, Neuron());
        }
    }
    //Topological sort
}

Network::Network(QString filename){
    Genome genome(filename);
    for(auto gene : genome.genes){
        if(!gene.enabled)
            continue;
        if(neurons.find(gene.into) != neurons.end()){
            neurons[gene.into].incoming.push_back(gene);
        }else{
            neurons.insert(gene.into, Neuron());
            neurons[gene.into].incoming.push_back(gene);
        }
        if(neurons.find(gene.out) == neurons.end()){
            neurons.insert(gene.out, Neuron());
        }
    }
    //Topological sort
}


double Network::evaluateNetwork(const QVector<double>& inputs){
    assert(AI::inputSize == inputs.size() - 1); //Because input size includes bias


}
