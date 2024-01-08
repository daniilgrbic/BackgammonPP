#include "genome.hpp"
#include "neat.hpp"
#include "connectgene.hpp"
#include <QString>
#include <algorithm>
#include <fstream>
#include <random>
#include <iostream>
#include <queue>
#include "engine/backgammon.h"
#include <atomic>
#include <thread>
#include <functional>

Genome::Genome(std::ifstream& filestream){
    if(!filestream.is_open())
        throw std::ios_base::failure("Cant open bot file");
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
        genes.push_back(ConnectGene(out, into, weight, enabled, innovation_num));
    }

}

void Genome::printToFile(std::string filename){
    std::ofstream filestream(filename);
    filestream << maxNeuron << " " << innovation->innovation << std::endl;
    int id = 1;
    for(auto gene : genes){
        filestream << id++ << " " << gene.out << " " << gene.into << " " << gene.weight <<  " " << gene.enabled << " " << gene.innovation << std::endl;
    }
}


void Genome::mutateAddConnection(){
    if(AI::random01(AI::generator) < AI::mutateConnectionsChance){
        std::vector<std::vector<int>> adj_list(maxNeuron);
        std::vector<int> in_degree(maxNeuron, 0);

        for(auto gene : genes){
            adj_list[gene.out].push_back(gene.into);
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
                    genes.push_back(ConnectGene(top_order[unbiasedOrder[i]], top_order[j], 1.0, true, innovation->newInnovation()));
                    return;
                }
            }
        }

    }
}
void Genome::mutateAddNode(){
    if(AI::random01(AI::generator) < AI::nodeMutationChance){
        std::vector<std::reference_wrapper<ConnectGene>> out;
        int nelems = 1;
        std::sample(genes.begin(), genes.end(), std::back_inserter(out), nelems, std::mt19937{std::random_device{}()});
        auto oldGene = out[0];
        oldGene.get().enabled = false;
        genes.push_back(ConnectGene(maxNeuron, oldGene.get().into, oldGene.get().weight, true, innovation->newInnovation()));
        genes.push_back(ConnectGene(oldGene.get().out, maxNeuron, 1.0, true, innovation->newInnovation()));

        ++maxNeuron;
    }
}
void Genome::mutateConnectionWeight(){
    for(auto& gene : genes){
        if(AI::random01(AI::generator) < AI::perturbChance){
            gene.weight = gene.weight + AI::random01(AI::generator) * AI::stepSize * 2 - AI::stepSize;
        }else{
            gene.weight = AI::random01(AI::generator) * 4 - 2;
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
    while(i < g1.genes.size() && i < g2.genes.size() && g1.genes[i].innovation == g2.genes[i].innovation){
        if(0.5 > AI::random01(AI::generator)){
            genes.push_back(ConnectGene(g1.genes[i]));

        }else{
            genes.push_back(ConnectGene(g2.genes[i]));
        }
        ++i;
    }
    if(g1.fitness > g2.fitness){
        while(i < g1.genes.size()){
            genes.push_back(ConnectGene(g1.genes[i]));
            ++i;
        }
    }else{
        while(i < g2.genes.size()){
            genes.push_back(ConnectGene(g2.genes[i]));
            ++i;
        }
    }

}
Genome::Genome(const Genome& genome){
    this->innovation = genome.innovation;
    this->maxNeuron = genome.maxNeuron;
    for(int i = 0; i < genome.genes.size(); ++i){
        this->genes.push_back(ConnectGene(genome.genes[i]));
    }
    this->fitness = genome.fitness;
}

double Genome::disjoint(const Genome& g1, const Genome& g2){
    size_t disjoint {0};

    size_t i {0};
    while(g1.genes[i].innovation == g2.genes[i].innovation){
        ++i;
    }
    disjoint += g1.genes.size() + g2.genes.size() - 2 * i;
    size_t N = std::max(g1.genes.size(), g2.genes.size());

    return static_cast<double>(disjoint) / N;

}
double Genome::weights(const Genome& g1, const Genome& g2){
    double sum {0.0};
    size_t i {0};
    while(g1.genes[i].innovation == g2.genes[i].innovation){
        sum += std::abs(g1.genes[i].weight - g2.genes[i].weight);
        ++i;
    }
    return sum / i;

}
bool Genome::sameSpecies(const Genome& g1, const Genome& g2){
    return AI::deltaDisjoint*disjoint(g1, g2) + AI::deltaWeights*weights(g1, g2) < AI::deltaTreshold;
}

void Genome::playBackgammon(Genome& g1, Genome& g2, std::atomic<int>& fit1, std::atomic<int>& fit2){
    Backgammon game;
    // game.currentRoll().onRoll() vraca igraca koji prvi igra
    // ovde eventualno obrnes ko je koji igrac kod ovih mreza i genoma
    // konvencija je WHITE ide 24->1, a BLACK 1->24
    Network n1(g1); //playercolor1
    Network n2(g2); //playercolor2
//    std::cout << "Its time to duel" << std::endl;
    while(true){
        std::vector<Turn> turns = game.generateLegalTurns();
        std::pair<double, Turn> bestTurn = {n1.evaluateNetwork(n1.inputFromState(turns[0].m_finalBoard)), turns[0]};
        for(const auto& turn : turns){
            BoardState b = turn.m_finalBoard;
            double eval = n1.evaluateNetwork(n1.inputFromState(b));
            if(bestTurn.first < eval){
                bestTurn.first = eval;
                bestTurn.second = turn;
            };
        }
        game.playTurn(bestTurn.second);
        if(game.isFinished(PlayerColor::WHITE)){
            ++fit1;
//            std::cout << std::this_thread::get_id() << " | game finished" << std::endl;
            return;
        }
        //TODO turns should be mirrored too
        turns = game.generateLegalTurns();
        bestTurn = {n2.evaluateNetwork(n2.inputFromState(turns[0].m_finalBoard.mirror())), turns[0]};
        for(const auto& turn : turns){
            BoardState b = turn.m_finalBoard;
            double eval = n2.evaluateNetwork(n2.inputFromState(b.mirror()));
            if(bestTurn.first < eval){
                bestTurn.first = eval;
                bestTurn.second = turn;};
        }
        game.playTurn(bestTurn.second);
        if(game.isFinished(PlayerColor::BLACK)){
            ++fit2;
//            std::cout << std::this_thread::get_id() << " | game finished" << std::endl;
            return;
        }
    }
}
