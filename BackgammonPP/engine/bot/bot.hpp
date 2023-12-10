#pragma once

#include "network.hpp"
#include "QString"
#include "engine/core/boardstate.h"

namespace AI{
    class Bot{
    public:
        Bot(QString filepath);
        ~Bot();
        Move bestMove(BoardState state);
        bool shouldDouble(BoardState state);
    private:
        Network* network;
    };
}
