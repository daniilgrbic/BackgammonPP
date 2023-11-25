#include "boardbar.h"

BoardBar::BoardBar(QGraphicsItem* parent, qreal width, qreal height)
    : QGraphicsItem(parent),
      m_width(width),
      m_height(height)
{

}

QRectF BoardBar::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}

void BoardBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(boundingRect());

}
