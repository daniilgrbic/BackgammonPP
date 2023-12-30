#pragma once

#include "playercolor.h"
#include "../../utility/serializable.h"

#include <optional>

using uint = unsigned int;

class Point : public Serializable
{
public:
    Point() = default;
    Point(std::optional<PlayerColor>, uint);
    std::optional<PlayerColor> owner() const;
    uint count() const;
    uint add(PlayerColor color, uint count = 1);
    uint remove(uint count = 1);

    static int idByPlayer(PlayerColor color, size_t index);
    static int centralMirrorId(size_t index);
    static int verticalMirrorId(size_t index);

    bool operator ==(const Point&) const = default;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);

private:
    std::optional<PlayerColor> m_owner;
    uint m_count = 0;
};
