#pragma once

#include "playercolor.h"
#include "move.h"
#include "boardstate.h"
#include "../../utility/serializable.h"

#include <algorithm>
#include <vector>

struct Turn : public Serializable
{
    Turn() = default;

    Turn(const PlayerColor player, const std::vector<int>& dice, const std::vector<Move>& moves, const BoardState& finalBoard)
        : m_player { player }, m_dice { dice }, m_moves { moves }, m_finalBoard { finalBoard }
    {}

    bool operator ==(const Turn& other) const {
        return m_finalBoard == other.m_finalBoard;
    }

    static std::vector<Turn> unique(const std::vector<Turn>& turns) {
        std::vector<Turn> reprs;
        for (const auto& turn : turns) {
            if (std::find(reprs.begin(), reprs.end(), turn) == reprs.end())
                reprs.push_back(turn);
        }

        return reprs;
    }

    PlayerColor m_player;
    std::vector<int> m_dice;
    std::vector<Move> m_moves;
    BoardState m_finalBoard;

    QString toString() const;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);
};
