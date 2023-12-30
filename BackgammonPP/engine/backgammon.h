#pragma once

#include "engine/core/game.h"

class Backgammon : public Game
{
public:
    Backgammon();

    bool isGammon() const;
    bool isBackgammon() const;

    // Game interface
    std::vector<Turn> generateLegalTurns() override;
    bool isFinished(PlayerColor player) const override;
    std::optional<GameResult> getResult() override;

    const int CHECKERS_COUNT = 15;
};
