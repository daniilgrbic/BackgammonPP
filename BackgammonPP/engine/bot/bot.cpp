#include "bot.hpp"
#include "engine/core/game.h"
#include "engine/core/turn.h"
#include "engine/core/playercolor.h"

namespace AI{

    Bot::Bot(std::string filepath){
        network = new Network(filepath);
    }
    Bot::~Bot(){
        delete network;
    }
    Turn Bot::bestTurn(PlayerColor color, const std::vector<Turn>& turns){
        if(color == PlayerColor::WHITE){
            std::pair<double, Turn> bestTurn = {network->evaluateNetwork(network->inputFromState(turns[0].m_finalBoard)), turns[0]};
            for(const auto& turn : turns){
                BoardState b = turn.m_finalBoard;
                double eval = network->evaluateNetwork(network->inputFromState(b));
                if(bestTurn.first < eval){
                    bestTurn.first = eval;
                    bestTurn.second = turn;
                };
            }
            return bestTurn.second;
        }else{
            std::pair<double, Turn> bestTurn = {network->evaluateNetwork(network->inputFromState(turns[0].m_finalBoard.mirror())), turns[0]};
            for(const auto& turn : turns){
                BoardState b = turn.m_finalBoard;
                double eval = network->evaluateNetwork(network->inputFromState(b.mirror()));
                if(bestTurn.first < eval){
                    bestTurn.first = eval;
                    bestTurn.second = turn;
                };
            }
            return bestTurn.second;
        }



    }
    bool Bot::shouldDouble(const Game& game, const BoardState& state){
        return true;
    }
}

