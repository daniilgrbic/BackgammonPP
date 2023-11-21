#pragma once

#include "playercolor.h"
#include "../../utility/serializable.h"

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
    std::optional<PlayerColor> m_owner;
    int m_count = 0;
};
