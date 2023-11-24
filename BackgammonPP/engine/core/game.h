#pragma once

#include "boardstate.h"
#include "gameresult.h"
#include "turn.h"
#include "roll.h"

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
    Roll m_currentRoll;
    std::vector<Turn> m_history;
    GameResult m_gameResult;
};
