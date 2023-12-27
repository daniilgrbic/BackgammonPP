#pragma once

#include "engine/core/game.h"

class LongNardy : public Game
{
public:
    enum class VictoryType;
    struct Result;

    LongNardy();

    static int pointIdByPlayer(PlayerColor player, int point);
    static BoardState mirrorBoard(BoardState boardState);
    static Move mirrorMove(Move move);

    // Game interface
    std::vector<Turn> generateLegalTurns() override;
    bool isFinished(PlayerColor player) const override;
    bool isBlockedBy(const Point& point, PlayerColor player) const override;
    std::optional<GameResult> getResult();

private:
    const int CHECKERS_COUNT = 15;
};