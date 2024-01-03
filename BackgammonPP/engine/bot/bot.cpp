#include "bot.hpp"
#include "engine/core/game.h"
#include "engine/core/turn.h"
#include "engine/core/playercolor.h"

namespace AI{

Bot::Bot(std::ifstream& filestream, QObject *parent) : QObject(parent){
        network = new Network(filestream);
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
    bool Bot::shouldDouble(PlayerColor color, const BoardState& state, bool onTurn){
        double magicConstant = onTurn ?  0.8 : 0.5;
        if(color == PlayerColor::WHITE){
            return network->evaluateNetwork(network->inputFromState(state)) > magicConstant;
        }else{
            return network->evaluateNetwork(network->inputFromState(state.mirror())) > magicConstant;
        }
    }


    void Bot::requestBestMove(PlayerColor color, const std::vector<Turn>& turns){
        emit bestMove(bestTurn(color, turns));
    }
}

