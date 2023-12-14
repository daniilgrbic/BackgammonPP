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
    int innov;
    filestream >> maxNeuron >> innov;
    innovation = new Innovation(innov);
    int id;
    int into;
    int out;
    double weight;
    bool enabled;
    int innovation_num;
    while(filestream >> id >> out >> into >> weight >> enabled >> innovation_num){
        genes.push_back(new ConnectGene(out, into, weight, enabled, innovation_num));
    }

}

void Genome::printToFile(std::string filename){
    std::ofstream filestream(filename);
    filestream << maxNeuron << " " << innovation->innovation << std::endl;
    int id = 1;
    for(auto gene : genes){
        filestream << id++ << " " << gene->out << " " << gene->into << " " << gene->weight <<  " " << gene->enabled << " " << gene->innovation << std::endl;
    }
}


void Genome::mutateAddConnection(){
    if(true or AI::random01(AI::generator) < AI::mutateConnectionsChance){
        std::vector<std::vector<int>> adj_list(maxNeuron);
        std::vector<int> in_degree(maxNeuron, 0);

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
                if(std::find(adj_list[first].begin() + j, adj_list[first].end(), top_order[j]) == adj_list[first].end() && j > AI::inputSize){
                    genes.push_back(new ConnectGene(top_order[unbiasedOrder[i]], top_order[j], 1.0, true, innovation->newInnovation()));
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
        genes.push_back(new ConnectGene(maxNeuron, oldGene->into, oldGene->weight, true, innovation->newInnovation()));
        genes.push_back(new ConnectGene(oldGene->out, maxNeuron, 1.0, true, innovation->newInnovation()));

        ++maxNeuron;
    }
}
void Genome::mutateConnectionWeight(){
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

Genome::Genome(const Genome& g1, const Genome& g2){
    this->innovation = g1.innovation;
    this->maxNeuron = std::max(g1.maxNeuron, g2.maxNeuron);
    int i=0;
    while(g1.genes[i]->innovation == g2.genes[i]->innovation){
        if(0.5 > AI::random01(AI::generator)){
            ConnectGene* newGene = new ConnectGene(g1.genes[i]);
            genes.push_back(newGene);

        }else{
            ConnectGene* newGene = new ConnectGene(g2.genes[i]);
            genes.push_back(newGene);
        }
        ++i;
    }
    if(0.5 > AI::random01(AI::generator)){//TODO change so that genome with better fitness is chosen
        while(i < g1.genes.size()){
            ConnectGene* newGene = new ConnectGene(g1.genes[i]);
            genes.push_back(newGene);
            ++i;
        }
    }else{
        while(i < g2.genes.size()){
            ConnectGene* newGene = new ConnectGene(g2.genes[i]);
            genes.push_back(newGene);
            ++i;
        }
    }


}
Genome::Genome(const Genome& genome){
    this->innovation = genome.innovation;
    this->maxNeuron = genome.maxNeuron;
    for(int i = 0; i < genome.genes.size(); ++i){
        this->genes.push_back(new ConnectGene(genome.genes[i]));
    }
}

double Genome::disjoint(const Genome& g1, const Genome& g2){
    size_t disjoint {0};

    size_t i {0};
    while(g1.genes[i]->innovation == g2.genes[i]->innovation){
        ++i;
    }
    disjoint += g1.genes.size() + g2.genes.size() - 2 * i;
    size_t N = std::max(g1.genes.size(), g2.genes.size());

    return static_cast<double>(disjoint) / N;

}
double Genome::weights(const Genome& g1, const Genome& g2){
    double sum {0.0};
    size_t i {0};
    while(g1.genes[i]->innovation == g2.genes[i]->innovation){
        sum += std::abs(g1.genes[i]->weight - g2.genes[i]->weight);
        ++i;
    }
    return sum / i;

}
bool Genome::sameSpecies(const Genome& g1, const Genome& g2){
    return AI::deltaDisjoint*disjoint(g1, g2) + AI::deltaWeights*weights(g1, g2) < AI::deltaTreshold;
}
