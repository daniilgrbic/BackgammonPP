#pragma once

#include "engine/core/game.h"

class LongNardy : public Game
{
public:
    LongNardy();

    // Game interface
    std::vector<Turn> generateLegalTurns() override;
    bool isFinished(PlayerColor player) const override;
    bool isBlockedBy(const Point& point, PlayerColor player) const override;
    std::optional<GameResult> getResult() override;

    const int CHECKERS_COUNT = 15;
};
