#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include <QVector>
#include "boarddie.h"

class BoardPlayingDie : public BoardDie
{
public:
    enum class Position {
        LEFT,
        RIGHT
    };
    BoardPlayingDie(QGraphicsItem *parent, qreal side, QMap<Position, QPointF> posMap, int value = 1);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void updateDie(Position pos, int value);
private:
    qreal m_dotSizeShare = 0.15;
    int m_value;
    qreal m_dotDiameter;

    Position m_position;
    QMap<Position, QPointF> m_posMap;

    const QVector<QPointF> dotPositions[6] = {
        { QPointF(0.50, 0.50) },
        { QPointF(0.25, 0.25), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.50, 0.50), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75), QPointF(0.50, 0.50) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75), QPointF(0.25, 0.50), QPointF(0.75, 0.50) },
    };
};
