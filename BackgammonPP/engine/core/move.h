#pragma once

#include "playercolor.h"
#include "specialposition.h"

#include <variant>

struct Move
{
    Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to);

    Move mirror() const;

    PlayerColor m_player;
    std::variant<int, SpecialPosition> m_from;
    std::variant<int, SpecialPosition> m_to;
};
