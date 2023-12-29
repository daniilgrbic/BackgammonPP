#pragma once

#include "engine/core/game.h"

class Backgammon : public Game
{
public:
    enum class VictoryType;
    struct Result;

    Backgammon();
    Backgammon(const BoardState& board, const Roll& roll);

    bool isGammon() const;
    bool isBackgammon() const;

    // Game interface
    std::vector<Turn> generateLegalTurns() override;
    bool isFinished(PlayerColor player) const override;
    std::optional<GameResult> getResult() override;
private:
    const int CHECKERS_COUNT = 15;
};
