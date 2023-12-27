#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "boarddie.h"
#include <QVector>

class BoardDoublingDie : public BoardDie
{
public:
    enum class Position {
        OUT,
        LEFT,
        RIGHT
    };
    BoardDoublingDie(QGraphicsItem *parent, qreal side, QMap<Position, QPointF> posMap,int value = 2);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void updateDie(Position pos, int value);
private:
    int m_value;
    QVector<int> validValues = {2, 4, 8, 16, 32, 64};

    Position m_position;
    QMap<Position, QPointF> m_posMap;
};
