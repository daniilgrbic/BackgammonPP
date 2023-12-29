#include "turn.h"

#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QJsonDocument>


QString Turn::toString() const
{
    QString builder = "";
    builder.append(m_player == PlayerColor::WHITE ? "W " : "B ");
    builder.append(QString("%1-%2:").arg(m_dice[0]).arg(m_dice[1]));
    for(auto& move : m_moves) {
        builder.append(" " + move.toString());
    }
    return builder;
}

QVariant Turn::toVariant() const
{
    QVariantMap map;
    map.insert("player", m_player == PlayerColor::WHITE ? "white" : "black");
    map.insert("dice", QVariantList(m_dice.begin(), m_dice.end()));
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
    m_player = data.value("player").toString().toStdString() == "white" ?
                   PlayerColor::WHITE : PlayerColor::BLACK;
    m_finalBoard.fromVariant(data.value("finalBoard"));

    QVariantList diceList = data.value("dice").toList();
    m_dice.resize(diceList.size());
    for(int i = 0; i < diceList.size(); i++) {
        m_dice[i] = diceList[i].toInt();
    }

    QVariantList movesList = data.value("moves").toList();
    m_moves.resize(movesList.size());
    for(int i = 0; i < movesList.size(); i++)
    {
        m_moves[i].fromVariant(movesList[i]);
    }
}
