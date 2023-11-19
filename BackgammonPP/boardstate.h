#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "point.h"
#include "playercolor.h"
#include "move.h"
#include "utility/serializable.h"

#include <vector>

constexpr int NUMBER_OF_POINTS {24};

class BoardState : public Serializable
{
public:
    BoardState();

    int bar(PlayerColor player) const;
    Point point(int pos) const;
    void move(const Move& move);

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);

private:
    std::vector<Point> points;
    int blackBar = 0;
    int whiteBar = 0;
    int blackOff = 0;
    int whiteOff = 0;
};

#endif // BOARDSTATE_H
