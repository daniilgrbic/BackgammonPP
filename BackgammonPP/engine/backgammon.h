#pragma once

#include "engine/core/game.h"
#include "engine/bot/progresssignaler.hpp"

class Backgammon : public Game
{
public:
    enum class VictoryType;
    struct Result;

    Backgammon();

    // Game interface
    std::vector<Turn> generateLegalTurns() override;

    bool isFinished(PlayerColor player) const override;
private:
    const int CHECKERS_COUNT = 15;
};
