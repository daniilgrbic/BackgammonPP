#include "boardstate.h"
#include "point.h"

#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>

Point::Point(std::optional<PlayerColor> owner, uint count)
    : m_owner(owner)
    , m_count(count)
    {};

auto Point::owner() const -> std::optional<PlayerColor>
{
    return m_owner;
}

auto Point::count() const -> uint
{
    return m_count;
}

auto Point::add(PlayerColor color, uint count) -> uint
{
    if(m_count > 0 and m_owner != color)
        throw std::logic_error("Cannot add checkers to point owned by opponent");

    m_count += count;
    m_owner = color;
    return m_count;
}

auto Point::remove(uint count) -> uint
{
    if(m_count < count)
        throw std::logic_error("Cannot remove more checkers than present on a point");

    m_count -= count;
    if(m_count == 0)
        m_owner.reset();
    return m_count;
}

auto Point::idByPlayer(PlayerColor color, size_t index) -> int
{
    if(index < 1 or index > NUMBER_OF_POINTS)
        throw std::logic_error("Point index must be an integer between 1 and 24");

    return color == PlayerColor::WHITE ? index : NUMBER_OF_POINTS + 1 - index;
}

auto Point::centralMirrorId(size_t index) -> int
{
    if(1 <= index and index <= 12) return index + 12;
    if(13 <= index and index <= 24) return index - 12;
    throw std::logic_error("Point index must be an integer between 1 and 24");
}

auto Point::verticalMirrorId(size_t index) -> int
{
    if(1 <= index and index <= 24) return NUMBER_OF_POINTS + 1 - index;
    throw std::logic_error("Point index must be an integer between 1 and 24");
}

auto Point::toVariant() const -> QVariant
{
    QVariantMap map;
    if(m_owner.has_value())
    {
        switch(m_owner.value())
        {
        case PlayerColor::BLACK:
            map.insert("owner", "black");
            break;
        case PlayerColor::WHITE:
            map.insert("owner", "white");
            break;
        }
    }
    map.insert("count", m_count);
    return map;
}

void Point::fromVariant(const QVariant &variant)
{
    QVariantMap data = variant.toMap();
    m_count = data.value("count").toInt();
    if(m_count)
    {
        if(data.value("owner").toString().toStdString() == "black")
        {
            m_owner = PlayerColor::BLACK;
        }
        else
        {
            m_owner = PlayerColor::WHITE;
        }
    }
    else
    {
        m_owner.reset();
    }
}
