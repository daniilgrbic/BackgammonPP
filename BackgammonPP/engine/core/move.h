#pragma once

#include "playercolor.h"
#include "specialposition.h"
#include "../../utility/serializable.h"

#include <variant>
#include <functional>


struct Move : public Serializable
{
    Move() = default;

    Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to, const bool isHit = false);

    Move mirror() const;
    static Move verticalMirror(Move move);
    static Move centralMirror(Move move);

    bool operator ==(const Move&) const = default;

    PlayerColor m_player;
    std::variant<int, SpecialPosition> m_from;
    std::variant<int, SpecialPosition> m_to;

    QString toString() const;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);
    bool m_isHit;
};

template <>
struct std::hash<Move>
{
      std::size_t operator()(const Move& move) const
      {
          // Used only as a key in unordered_map with ~10 elements
          return 0;
      }
};
