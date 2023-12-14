#pragma once

#include "playercolor.h"
#include "move.h"
#include "boardstate.h"

#include <vector>

struct Turn
{
    Turn(const unsigned int index, const PlayerColor player, const std::vector<Move>& moves, const BoardState& finalBoard)
        : m_index { index }, m_player { player }, m_moves { moves }, m_finalBoard { finalBoard }
    {}

    unsigned int m_index;
    PlayerColor m_player;
    std::vector<Move> m_moves;
    BoardState m_finalBoard;
};
