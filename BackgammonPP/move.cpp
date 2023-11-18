#include "move.h"

Move::Move(
    const PlayerColor& who,
    const std::variant<int, SpecialPosition>& from,
    const std::variant<int, SpecialPosition>& to
    )
    : who { who }, from { from }, to { to }
{}
