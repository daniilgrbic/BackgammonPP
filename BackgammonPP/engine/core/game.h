#pragma once

#include "boardstate.h"
#include "gameresult.h"
#include "turn.h"
#include "roll.h"

#include <vector>
#include <optional>

class Game
{
public:
    virtual ~Game() = 0;
    virtual std::vector<Turn> generateLegalTurns() = 0;
    void playTurn(Turn turn);
    std::optional<GameResult> getResult() const;
    virtual bool isFinished(PlayerColor player) const = 0;

protected:
    Game();
    bool isBlot(int point, PlayerColor player) const;
    bool isBlocked(int point, PlayerColor player) const;
    bool isBearingOff(PlayerColor player) const;

    BoardState m_board;
    Roll m_currentRoll;
    std::vector<Turn> m_history;
    std::optional<GameResult> m_result;

// private:
//     const int CHECKERS_COUNT;
};
