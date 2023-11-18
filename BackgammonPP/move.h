#ifndef MOVE_H
#define MOVE_H

#include "playercolor.h"
#include "specialposition.h"

#include <variant>

class Move
{
public:
    Move(const PlayerColor& who, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to);

private:
    PlayerColor who;
    std::variant<int, SpecialPosition> from;
    std::variant<int, SpecialPosition> to;
};

#endif // MOVE_H
