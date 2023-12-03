#pragma once

#include "playercolor.h"
#include "specialposition.h"

#include <variant>

struct Move
{
    PlayerColor player;
    std::variant<int, SpecialPosition> from;
    std::variant<int, SpecialPosition> to;
};
