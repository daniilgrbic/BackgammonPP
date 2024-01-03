#pragma once

#include "network.hpp"
#include "QString"
#include "engine/core/boardstate.h"
#include "engine/core/game.h"
#include "engine/core/turn.h"
#include "engine/core/playercolor.h"
#include <QObject>

namespace AI{
    class Bot : public QObject{
        Q_OBJECT

    public:
        Bot(std::ifstream& stream, QObject *parent = nullptr);
        ~Bot();
        Turn bestTurn(PlayerColor color, const std::vector<Turn>& turns);
        bool shouldDouble(PlayerColor color, const BoardState& state, bool onTurn);
    signals:
        void bestMove(Turn turn);
    public slots:
        void requestBestMove(PlayerColor color, const std::vector<Turn>& turns);
    private:
        Network* network;
    };
}
