#pragma once

#include "playercolor.h"
#include "specialposition.h"
#include "../../utility/serializable.h"

#include <variant>
#include <functional>

struct Move : public Serializable
{
    Move() = default;

    Move(
        const PlayerColor player,
        const std::variant<int,SpecialPosition>& from,
        const std::variant<int, SpecialPosition>& to,
        const bool isHit = false
    );

    Move(
        const PlayerColor player,
        const std::variant<int,SpecialPosition>& from,
        const std::variant<int, SpecialPosition>& to,
        const std::vector<int>& hittedPoints
    );

    Move mirror() const;

    bool operator ==(const Move&) const = default;

    PlayerColor m_player;
    std::variant<int, SpecialPosition> m_from;
    std::variant<int, SpecialPosition> m_to;
    std::vector<int> m_hittedPoints;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);
};

// TODO
template <>
struct std::hash<Move>
{
    std::size_t operator()(const Move& move) const
    {
        size_t hashValue = 0u;
        if (const auto* pos = std::get_if<int>(&move.m_from)) {
            hashValue |= *pos;
        } else if (const auto* pos = std::get_if<SpecialPosition>(&move.m_from)) {
            if (*pos == SpecialPosition::BAR)
                hashValue |= (1 << 5);
            else
                hashValue |= (1 << 6);
        }

        if (const auto* pos = std::get_if<int>(&move.m_to)) {
            hashValue |= (*pos << 7);
        } else if (const auto* pos = std::get_if<SpecialPosition>(&move.m_to)) {
            if (*pos == SpecialPosition::BAR)
                hashValue |= (1 << 12);
            else
                hashValue |= (1 << 13);
        }

        hashValue |= (move.m_hittedPoints.size() << 14);
        if (move.m_player == PlayerColor::BLACK)
            hashValue |= (1 << 16);


        return hashValue;
    }
};
