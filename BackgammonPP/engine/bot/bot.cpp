#include "bot.hpp"
#include "engine/core/game.h"

namespace AI{

    Bot::Bot(QString filepath){
        network = new Network(filepath);
    }
    Bot::~Bot(){
        delete network;
    }
    Move Bot::bestMove(BoardState state){

    }
    bool Bot::shouldDouble(BoardState state){

    }
}

