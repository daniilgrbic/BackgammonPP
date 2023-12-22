#pragma once

#include "playercolor.h"
#include "specialposition.h"

#include <variant>
#include <functional>


struct Move
{
    Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to, const bool isHit = false);

    Move mirror() const;

    bool operator ==(const Move&) const = default;

    PlayerColor m_player;
    std::variant<int, SpecialPosition> m_from;
    std::variant<int, SpecialPosition> m_to;
    bool m_isHit;
};

// TODO
template <>
struct std::hash<Move>
{
  std::size_t operator()(const Move& move) const
  {
      return 0;
  }
};
