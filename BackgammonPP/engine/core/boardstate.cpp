#include "boardstate.h"

#include <algorithm>

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>

BoardState::BoardState()
    : m_points(std::array<Point, NUMBER_OF_POINTS>())
{}

BoardState::BoardState(
    const std::vector<std::pair<int, int>>& whitePoints,
    const std::vector<std::pair<int, int>>& blackPoints,
    const int whiteBar, const int blackBar,
    const int whiteOff, const int blackOff
)
    : m_whiteBar { whiteBar }, m_blackBar { blackBar }
    , m_whiteOff { whiteOff }, m_blackOff { blackOff }
    , m_points {}
{
    auto whiteCheckers = whiteBar + whiteOff;
    for (const auto [pos, count] : whitePoints) {
        point(pos).add(PlayerColor::WHITE, count);
        whiteCheckers += count;
    }

    auto blackCheckers = blackBar + blackOff;
    for (const auto [pos, count] : blackPoints) {
        point(pos).add(PlayerColor::BLACK, count);
        blackCheckers += count;
    }
}

int BoardState::bar(PlayerColor player) const {
    return player == PlayerColor::BLACK ? m_blackBar : m_whiteBar;
}

int BoardState::off(PlayerColor player) const {
    return player == PlayerColor::BLACK ? m_blackOff : m_whiteOff;
}

Point& BoardState::point(const int pos) {
    assert(pos >= 1 and pos <= NUMBER_OF_POINTS);
    return m_points[pos - 1];
}

const Point& BoardState::point(const int pos) const {
    assert(pos >= 1 and pos <= NUMBER_OF_POINTS);
    return m_points[pos - 1];
}

void BoardState::move(const Move &move)
{
    const auto& player = move.m_player;
    const auto& from = move.m_from;
    const auto& to = move.m_to;

    if (move.m_isHit) {
        const auto opponent = player == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE;
        auto opponentsMove = Move(opponent, to, SpecialPosition::BAR);
        this->move(opponentsMove);
    }

    auto& bar = player == PlayerColor::BLACK ? m_blackBar : m_whiteBar;
    auto& off = player == PlayerColor::BLACK ? m_blackOff : m_whiteOff;

    if (const int* fromPos = std::get_if<int>(&from)) {
        point(*fromPos).remove();
    } else if (std::get<SpecialPosition>(from) == SpecialPosition::BAR) {
        assert(bar >= 1);
        bar -= 1;
    } else {
        assert(false);
    }

    if (const int* toPos = std::get_if<int>(&to)) {
        point(*toPos).add(player);
    } else if (std::get<SpecialPosition>(to) == SpecialPosition::BAR) {
        bar += 1;
    } else {
        off += 1;
    }
}

BoardState BoardState::mirror() const {
    auto nextState = *this;
    std::reverse(nextState.m_points.begin(), nextState.m_points.end());
    return nextState;
}

BoardState BoardState::verticalMirror(BoardState board)
{
    std::reverse(board.m_points.begin(), board.m_points.end());
    return board;
}

BoardState BoardState::centralMirror(BoardState board)
{
    for(int i = 1; i <= 12; i++) {
        std::swap(board.point(i), board.point(i+12));
    }
    return board;
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

    QVariantList pointList = map.value("points").toList();
    for(int i = 0; i < NUMBER_OF_POINTS; i++)
    {
        m_points[i].fromVariant(pointList[i]);
    }
}
