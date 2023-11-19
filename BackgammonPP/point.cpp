#include "point.h"

#include <QVariant>
#include <QVariantMap>
#include <QJsonDocument>

Point::Point() = default;

std::optional<PlayerColor> Point::whosOwner() const
{
    return owner;
}

int Point::countChecker() const
{
    return count;
}

QVariant Point::toVariant() const
{
    QVariantMap map;
    if(owner.has_value())
    {
        switch(owner.value())
        {
        case PlayerColor::BLACK:
            map.insert("owner", "black");
            break;
        case PlayerColor::WHITE:
            map.insert("owner", "white");
            break;
        }
    }
    map.insert("count", count);
    return map;
}

void Point::fromVariant(const QVariant &variant)
{
    QVariantMap data = variant.toMap();
    count = data.value("count").toInt();
    if(count)
    {
        if(data.value("owner").toString().toStdString() == "black")
        {
            owner = PlayerColor::BLACK;
        }
        else
        {
            owner = PlayerColor::WHITE;
        }
    }
    else
    {
        owner.reset();
    }
}
