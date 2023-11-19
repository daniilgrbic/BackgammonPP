#include "boardstate.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>

BoardState::BoardState()
    : points(std::vector<Point>(NUMBER_OF_POINTS))
{}

int BoardState::bar(PlayerColor player) const {
    return player == PlayerColor::BLACK ? blackBar : whiteBar;
}

Point BoardState::point(const int pos) const {
    return points[pos];
}

QVariant BoardState::toVariant() const
{
    QVariantMap map;
    map.insert("blackBar", blackBar);
    map.insert("whiteBar", whiteBar);
    map.insert("blackOff", blackOff);
    map.insert("whiteOff", whiteOff);

    QVariantList pointList;
    for(const auto &point : points)
    {
        pointList.append(point.toVariant());
    }
    map.insert("points", pointList);
    return map;
}

void BoardState::fromVariant(const QVariant &variant)
{
    QVariantMap map = variant.toMap();
    blackBar = map.value("blackBar").toInt();
    whiteBar = map.value("whiteBar").toInt();
    blackOff = map.value("blackOff").toInt();
    whiteOff = map.value("whiteOff").toInt();
    points.clear();
    for(auto &pointVar : map.value("points").toList())
    {
        Point point;
        point.fromVariant(pointVar);
        points.push_back(point);
    }
}
