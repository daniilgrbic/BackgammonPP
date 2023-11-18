#ifndef POINT_H
#define POINT_H

#include "playercolor.h"

#include <optional>

class Point
{
public:
    Point();

    std::optional<PlayerColor> whosOwner() const;
    int countChecker() const;

private:
    int value;
    std::optional<PlayerColor> owner;
    int count;
};

#endif // POINT_H
