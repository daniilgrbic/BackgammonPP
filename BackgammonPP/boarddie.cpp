#include "boarddie.h"

BoardDie::BoardDie(QGraphicsItem *parent, qreal side)
    : QGraphicsItem(parent),
      m_side(side)
{
}

QRectF BoardDie::boundingRect() const {
    return QRectF(0, 0, m_side, m_side);
}
