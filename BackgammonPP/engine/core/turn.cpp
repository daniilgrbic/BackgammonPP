#include "turn.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>


QVariant Turn::toVariant() const
{
    QVariantMap map;
    map.insert("index", m_index);
    map.insert("player", m_player == PlayerColor::WHITE ? "white" : "black");
    map.insert("finalBoard", m_finalBoard.toVariant());

    QVariantList movesList;
    for(const auto &move : m_moves) {
        movesList.append(move.toVariant());
    }
    map.insert("moves", movesList);
    return map;
}

void Turn::fromVariant(const QVariant &variant)
{
    QVariantMap data = variant.toMap();
    m_index = data.value("index").toInt();
    m_player = data.value("player").toString().toStdString() == "white" ?
                   PlayerColor::WHITE : PlayerColor::BLACK;
    m_finalBoard.fromVariant(data.value("finalBoard"));

    QVariantList movesList = data.value("moves").toList();
    m_moves.resize(movesList.size());
    for(int i = 0; i < movesList.size(); i++)
    {
        m_moves[i].fromVariant(movesList[i]);
    }
}
