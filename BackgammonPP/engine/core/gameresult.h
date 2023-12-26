#pragma once

#include "playercolor.h"

struct GameResult {
    PlayerColor winner;
    int points;

    GameResult(PlayerColor winner, int points)
        : winner { winner }
        , points { points }
    {}
};
