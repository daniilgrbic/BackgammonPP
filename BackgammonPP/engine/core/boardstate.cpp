#include "boardstate.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>

BoardState::BoardState()
    : m_points(std::array<Point, NUMBER_OF_POINTS>())
{}

int BoardState::bar(PlayerColor player) const {
    return player == PlayerColor::BLACK ? m_blackBar : m_whiteBar;
}

Point& BoardState::point(const int pos) {
    assert(pos >= 0 and pos < NUMBER_OF_POINTS);
    return m_points[pos];
}

const Point& BoardState::point(const int pos) const {
    assert(pos >= 0 and pos < NUMBER_OF_POINTS);
    return m_points[pos];
}

void BoardState::move(const Move &move)
{
    (void) move;
}

BoardState BoardState::getNextState(const Move& move) const {
    auto nextState = *this;
    nextState.move(move);
    return nextState;
}

BoardState BoardState::getNextState(const std::vector<Move>& moves) const {
    auto nextState = *this;
    for (auto& move : moves) {
        nextState.move(move);
    }
    return nextState;
}

QVariant BoardState::toVariant() const
{
    QVariantMap map;
    map.insert("blackBar", m_blackBar);
    map.insert("whiteBar", m_whiteBar);
    map.insert("blackOff", m_blackOff);
    map.insert("whiteOff", m_whiteOff);

    QVariantList pointList;
    for(const auto &point : m_points)
    {
        pointList.append(point.toVariant());
    }
    map.insert("points", pointList);
    return map;
}

void BoardState::fromVariant(const QVariant &variant)
{
    QVariantMap map = variant.toMap();
    m_blackBar = map.value("blackBar").toInt();
    m_whiteBar = map.value("whiteBar").toInt();
    m_blackOff = map.value("blackOff").toInt();
    m_whiteOff = map.value("whiteOff").toInt();
    // m_points.clear();
    for(auto &pointVar : map.value("points").toList())
    {
        Point point;
        point.fromVariant(pointVar);
        // m_points.push_back(point);
    }
}
