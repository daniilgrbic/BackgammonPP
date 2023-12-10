#include "boarddice.h"

BoardDice::BoardDice(QGraphicsItem *parent, qreal side, int value)
    : QGraphicsItem(parent),
      m_side(side),
      m_value(value),
      m_dotDiameter(m_dotSizeShare * m_side)
{
}


void BoardDice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
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

QRectF BoardDice::boundingRect() const {
    return QRectF(0, 0, m_side, m_side);
}
