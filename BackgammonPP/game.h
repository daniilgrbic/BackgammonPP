#ifndef GAME_H
#define GAME_H

#include "boardstate.h"
#include "playercolor.h"
#include "roll.h"
#include "move.h"

#include <vector>

class Game
{
public:
    Game();

    std::vector<std::vector<Move>> generateLegalMoves();

private:
    PlayerColor black;
    PlayerColor white;

    Die firstDie {};
    Die secondDie {};

    BoardState state;

    Roll currentRoll;
};

#endif // GAME_H
