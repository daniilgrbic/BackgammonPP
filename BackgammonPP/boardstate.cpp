#include "boardstate.h"

// TODO
BoardState::BoardState()
{
}

int BoardState::bar(PlayerColor player) const {
    return player == PlayerColor::BLACK ? blackBar : whiteBar;
}

Point BoardState::point(const int pos) const {
    return points[pos];
}
