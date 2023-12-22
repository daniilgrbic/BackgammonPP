#pragma once

#include "playercolor.h"
#include "specialposition.h"
#include "../../utility/serializable.h"

#include <variant>

struct Move : public Serializable
{
    Move() = default;

    Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to);

    Move mirror() const;

    bool operator ==(const Move&) const = default;

    PlayerColor m_player;
    std::variant<int, SpecialPosition> m_from;
    std::variant<int, SpecialPosition> m_to;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);
};
