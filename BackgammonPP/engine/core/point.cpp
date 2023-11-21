#include "point.h"

#include <cassert>

#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>

Point::Point() = default;

std::optional<PlayerColor> Point::owner() const
{
    return m_owner;
}

uint Point::count()
{
    return m_count;
}

uint Point::add(PlayerColor color, uint count)
{
    assert(m_count == 0 or m_owner == color);
    m_count += count;
    m_owner = color;
    return m_count;
}

uint Point::remove(uint count)
{
    assert(m_count >= count);
    m_count -= count;
    if(m_count == 0)
        m_owner.reset();
    return m_count;
}

// TODO
size_t Point::idByPlayer(PlayerColor color, size_t index)
{
    return 0;
}

QVariant Point::toVariant() const
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
