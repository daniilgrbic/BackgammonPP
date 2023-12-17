#pragma once

#include "playercolor.h"
#include "../../utility/serializable.h"

#include <optional>
#include <vector>

using uint = unsigned int;

class Point : public Serializable
{
public:
    Point();
    std::optional<PlayerColor> owner() const;
    uint count() const;
    uint add(PlayerColor color, uint count = 1);
    uint remove(uint count = 1);

    // [[maybe unused]]
    static int idByPlayer(PlayerColor color, size_t index);

    bool operator ==(const Point&) const = default;

    // Serializable interface
    QVariant toVariant() const;
    void fromVariant(const QVariant &variant);

private:
    std::optional<PlayerColor> m_owner;
    uint m_count = 0;
};
