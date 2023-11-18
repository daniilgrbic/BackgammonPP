#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "point.h"
#include "playercolor.h"
#include "move.h"

#include <vector>

constexpr int NUMBER_OF_POINTS {24};

class BoardState
{
public:
    BoardState();

    int bar(PlayerColor player) const;

    Point point(int pos) const;

    void move(const Move& move);

private:
    std::vector<Point> points;
    int blackBar;
    int whiteBar;
    int blackOff;
    int whiteOff;
};

#endif // BOARDSTATE_H
