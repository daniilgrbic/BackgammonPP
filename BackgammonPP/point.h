#ifndef POINT_H
#define POINT_H

#include "playercolor.h"
#include "utility/serializable.h"

#include <optional>

class Point : public Serializable
{
public:
    Point();
    std::optional<PlayerColor> whosOwner() const;
    int countChecker() const;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);

private:
    std::optional<PlayerColor> owner;
    int count = 0;
};

#endif // POINT_H
