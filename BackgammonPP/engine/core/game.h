#pragma once

#include "boardstate.h"
#include "turn.h"
#include "gameresult.h"

#include <vector>
#include <variant>

class Game
{
public:
    Game();
    virtual std::vector<Turn> generateLegalTurns() = 0;
    GameResult playTurn(Turn turn);

protected:
    virtual GameResult checkFinished() = 0;

    BoardState m_board;
    std::vector<Turn> m_history;
    GameResult m_gameResult;
};
