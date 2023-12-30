#include "boardplayingdie.h"
#include <iostream>
BoardPlayingDie::BoardPlayingDie(QGraphicsItem *parent, qreal side, QMap<Position, QPointF> posMap, int value)
    : BoardDie(parent, side),
      m_value(value),
      m_dotDiameter(m_dotSizeShare * m_side),
      m_posMap(posMap)
{
}


void BoardPlayingDie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF drawRect = boundingRect();
    painter->setBrush(QBrush(m_secondary));
    painter->setPen(QPen(m_primary, 1.0));
    painter->drawRect(drawRect);

    painter->setBrush(QBrush(m_primary));
    for(auto &point: dotPositions[m_value - 1]){
        qreal x = point.x() * drawRect.width() - 0.5 * m_dotDiameter;
        qreal y = point.y() * drawRect.height() - 0.5 * m_dotDiameter;
        painter->drawEllipse(x, y, m_dotDiameter, m_dotDiameter);
    }
}

void BoardPlayingDie::updateDie(Position pos, int value){
    setPos(m_posMap[pos]);
    m_value = value;
    update();
    show();
}

