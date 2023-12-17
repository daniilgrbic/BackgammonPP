#pragma once

#include "point.h"
#include "playercolor.h"
#include "move.h"
#include "../../utility/serializable.h"

#include <array>
#include <vector>

constexpr int NUMBER_OF_POINTS {24};

class BoardState : public Serializable
{
public:
    BoardState();

    int bar(PlayerColor player) const;
    int off(PlayerColor player) const;

    Point& point(const int pos);
    const Point& point(const int pos) const;
    void move(const Move& move);

    BoardState mirror() const;

    BoardState getNextState(const Move& move) const;
    BoardState getNextState(const std::vector<Move>& move) const;

    bool operator ==(const BoardState&) const = default;

    // Serializable interface
    QVariant toVariant() const override;
    void fromVariant(const QVariant &variant) override;

private:
    std::array<Point, NUMBER_OF_POINTS> m_points;
    int m_blackBar = 0;
    int m_whiteBar = 0;
    int m_blackOff = 0;
    int m_whiteOff = 0;
};
