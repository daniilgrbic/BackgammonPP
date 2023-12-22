#pragma once

#include "network.hpp"
#include "QString"
#include "engine/core/boardstate.h"
#include "engine/core/game.h"
#include "engine/core/turn.h"
#include "engine/core/playercolor.h"

namespace AI{
    class Bot{
    public:
        Bot(std::string filepath);
        ~Bot();
        Turn bestTurn(PlayerColor color, const std::vector<Turn>& turns);
        bool shouldDouble(PlayerColor color, const BoardState& state, bool onTurn);
    private:
        Network* network;
    };
}
