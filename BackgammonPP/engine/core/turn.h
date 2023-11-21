#pragma once

#include "playercolor.h"
#include "move.h"

#include <vector>

struct Turn
{
    unsigned int index;
    PlayerColor player;
    std::vector<Move> moves;
};
