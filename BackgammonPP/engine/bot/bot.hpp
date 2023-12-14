#pragma once

#include "network.hpp"
#include "QString"
#include "engine/core/boardstate.h"
#include "engine/core/game.h"
#include "engine/core/turn.h"

namespace AI{
    class Bot{
    public:
        Bot(std::string filepath);
        ~Bot();
        Turn bestTurn(const Game& game, const BoardState& state);
        bool shouldDouble(const Game& game, const BoardState& state);
    private:
        Network* network;
    };
}
