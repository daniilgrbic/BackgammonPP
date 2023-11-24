#pragma once

#include "core/game.h"

class Backgammon : public Game
{
public:
    Backgammon();

    // Game interface
public:
    std::vector<Turn> generateLegalTurns() override;

private:
    GameResult checkFinished() override;

    bool isBlot(int point, PlayerColor player) const;
    bool isBlocked(int point, PlayerColor player) const;
    bool isBearingOff(PlayerColor player) const;
};
