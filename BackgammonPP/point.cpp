#include "point.h"

#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>

Point::Point() = default;

std::optional<PlayerColor> Point::whosOwner() const
{
    return m_owner;
}

int Point::countChecker() const
{
    return m_count;
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
