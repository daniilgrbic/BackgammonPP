#include "bot.hpp"
#include "engine/core/game.h"
#include "engine/core/turn.h"

namespace AI{

    Bot::Bot(std::string filepath){
        network = new Network(filepath);
    }
    Bot::~Bot(){
        delete network;
    }
    Turn Bot::bestTurn(const Game& game, const BoardState& state){

    }
    bool Bot::shouldDouble(const Game& game, const BoardState& state){

    }
}

