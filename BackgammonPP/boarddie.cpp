#include "boarddie.h"

BoardDie::BoardDie(QGraphicsItem *parent, qreal side)
    : QGraphicsItem(parent),
      m_side(side)
{
}

auto BoardDie::boundingRect() const -> QRectF {
    return {0, 0, m_side, m_side};
}
