#include "genome.hpp"
#include "neat.hpp"
#include "connectgene.hpp"
#include <QString>
#include <algorithm>
#include <fstream>
#include <random>
#include <iostream>
#include <queue>

Genome::Genome(std::string filename){
    std::ifstream filestream(filename);
    int id;
    int into;
    int out;
    double weight;
    bool enabled;
    int innovation_num;
    while(filestream >> id >> out >> into >> weight >> enabled >> innovation_num){
        genes.push_back(new ConnectGene(out, into, weight, enabled, innovation_num));

        if(out > maxNeuron){
            maxNeuron = out + 1;
        }
        if(into > maxNeuron){
            maxNeuron = into + 1;
        }
    }

}

void Genome::printToFile(std::string filename){
    std::ofstream filestream(filename);
    int id = 1;
    for(auto gene : genes){
        filestream << id++ << " " << gene->out << " " << gene->into << " " << gene->weight <<  " " << gene->enabled << " " << gene->innovation.innovation << std::endl;
    }
}


void Genome::mutateAddConnection(){
    if(true or AI::random01(AI::generator) < AI::mutateConnectionsChance){
        std::vector<std::vector<int>> adj_list(maxNeuron);
        std::vector<int> in_degree(maxNeuron, 0);
//        std::cout << maxNeuron << std::endl;

        for(auto gene : genes){
            adj_list[gene->out].push_back(gene->into);
        }

        for (int u = 0; u < maxNeuron; u++) {
            std::vector<int>::iterator itr;
            for (itr = adj_list[u].begin();
                 itr != adj_list[u].end(); itr++)
                in_degree[*itr]++;
        }

        std::queue<int> q;
        for (int i = 0; i < maxNeuron; i++)
            if (in_degree[i] == 0)
                q.push(i);

        int cnt = 0;

        std::vector<int> top_order;

        while(!q.empty()){
            int u = q.front();
            q.pop();
            top_order.push_back(u);

            std::vector<int>::iterator itr;
            for (itr = adj_list[u].begin();itr != adj_list[u].end(); itr++){
                if (--in_degree[*itr] == 0){
                     q.push(*itr);
                }
            }

            cnt++;

        }
        if (cnt != maxNeuron) {
            std::cout << "There exists a cycle in the graph\n";
            std::cout << cnt << std::endl;
            return;
        }

        for (int i = 0; i < top_order.size(); i++)
            std::cout << top_order[i] << " ";
        std::cout << std::endl;
        int startValue = 0;
        std::vector<int> unbiasedOrder(top_order.size());
        std::generate (
            unbiasedOrder.begin(),
            unbiasedOrder.end(),
            [&](){
                return startValue++;
            });
        std::shuffle(unbiasedOrder.begin(), unbiasedOrder.end(), AI::generator);

        for(int i = 0; i < top_order.size(); ++i){
            int first = top_order[unbiasedOrder[i]];
            for(int j = unbiasedOrder[i] + 1; j < top_order.size(); ++j){
                if(std::find(adj_list[first].begin() + j, adj_list[first].end(), top_order[j]) == adj_list[first].end()){
                    std::cout << "adding :" << top_order[unbiasedOrder[i]] << " -> " << top_order[j] << std::endl;
                    std::cout << "---------------------------------------------------------------------------" << std::endl;
                    genes.push_back(new ConnectGene(top_order[j], top_order[unbiasedOrder[i]], 1.0, true, 0));
                    return;
                }
            }
        }

    }
}
void Genome::mutateAddNode(){
    if(true or AI::random01(AI::generator) < AI::nodeMutationChance){
        std::vector<ConnectGene *> out;
        int nelems = 1;
        std::sample(genes.begin(), genes.end(), std::back_inserter(out), nelems, std::mt19937{std::random_device{}()});
        auto oldGene = out[0];
        oldGene->enabled = false;
        genes.push_back(new ConnectGene(maxNeuron, oldGene->out, oldGene->weight, true, oldGene->innovation.innovation + 1));
        genes.push_back(new ConnectGene(oldGene->into, maxNeuron, 1.0, true, oldGene->innovation.innovation + 2));

        ++maxNeuron;
    }
}
void Genome::mutateConnectionWeight(){
    //prodji sve gene i azuriraj tezine
    for(auto gene : genes){
        if(true or AI::random01(AI::generator) < AI::perturbChance){
            gene->weight = gene->weight + AI::random01(AI::generator) * AI::stepSize * 2 - AI::stepSize;
        }else{
            gene->weight = AI::random01(AI::generator) * 4 - 2;
        }
    }
}
void Genome::mutateEnableConnection(){
    //nista za sad
}
void Genome::mutateDisableConnection(){
    //nista za sad
}
void Genome::mutate(){
    mutateAddNode();
    mutateConnectionWeight();
    mutateAddConnection();
}
