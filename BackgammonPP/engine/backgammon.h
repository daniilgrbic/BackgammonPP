#pragma once

#include "engine/core/game.h"

class Backgammon : public Game
{
public:
    enum class VictoryType;
    struct Result;

    Backgammon();

    // Game interface
    std::vector<Turn> generateLegalTurns() override;
    bool isFinished(PlayerColor player) const override;

    static BoardState mirrorBoard(const BoardState& board);

private:
    const int CHECKERS_COUNT = 15;
};
