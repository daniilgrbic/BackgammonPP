#include "move.h"
#include "boardstate.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>

Move::Move(const PlayerColor player, const std::variant<int, SpecialPosition>& from, const std::variant<int, SpecialPosition>& to)
    : m_player { player }, m_from { from }, m_to { to }
{}

Move Move::mirror() const {
    std::variant<int, SpecialPosition> from;
    if (const int* fromPos = std::get_if<int>(&m_from)) {
        from = NUMBER_OF_POINTS + 1 - *fromPos;
    } else {
        from = m_from;
    }

    std::variant<int, SpecialPosition> to;
    if (const int* toPos = std::get_if<int>(&m_to)) {
        to = NUMBER_OF_POINTS + 1 - *toPos;
    } else {
        to = m_to;
    }

    return { m_player, from, to };
}

QVariant Move::toVariant() const
{
    QVariantMap map;
    map.insert("player", m_player == PlayerColor::WHITE ? "white" : "black");
    if(std::holds_alternative<int>(m_from)) {
        map.insert("from", std::get<int>(m_from));
    }
    else {
        switch(std::get<SpecialPosition>(m_from)) {
        case SpecialPosition::BAR:
            map.insert("from", "bar");
            break;
        case SpecialPosition::OFF:
            map.insert("from", "off");
            break;
        }
    }
    if(std::holds_alternative<int>(m_to)) {
        map.insert("to", std::get<int>(m_to));
    }
    else {
        switch(std::get<SpecialPosition>(m_to)) {
        case SpecialPosition::BAR:
            map.insert("to", "bar");
            break;
        case SpecialPosition::OFF:
            map.insert("to", "off");
            break;
        }
    }
    return map;
}

void Move::fromVariant(const QVariant &variant)
{
    QVariantMap data = variant.toMap();
    m_player = data.value("player").toString().toStdString() == "white" ?
                   PlayerColor::WHITE : PlayerColor::BLACK;
    if(data.value("from").toInt()) {
        m_from = data.value("from").toInt();
    }
    else {
        auto value = data.value("from").toString().toStdString();
        if(value == "bar") m_from = SpecialPosition::BAR;
        if(value == "off") m_from = SpecialPosition::OFF;
    }
    if(data.value("to").toInt()) {
        m_to = data.value("to").toInt();
    }
    else {
        auto value = data.value("to").toString().toStdString();
        if(value == "bar") m_to = SpecialPosition::BAR;
        if(value == "off") m_to = SpecialPosition::OFF;
    }
}
