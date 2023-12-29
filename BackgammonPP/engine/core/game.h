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
    virtual std::optional<GameResult> getResult() = 0;
    virtual bool isFinished(PlayerColor player) const = 0;

    const Roll& currentRoll() const;
    const BoardState& board() const;

protected:
    Game();
    bool isBlot(const Point& point, PlayerColor player) const;
    virtual bool isBlockedBy(const Point& point, PlayerColor player) const;
    virtual bool isBearingOff(const BoardState& board, PlayerColor player) const;

    BoardState m_board;
    Roll m_currentRoll;
    std::vector<Turn> m_history {};
    std::optional<GameResult> m_result { std::nullopt };

    Die m_firstDie {};
    Die m_secondDie {};
};
