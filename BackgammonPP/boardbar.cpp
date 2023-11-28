#include "boardbar.h"

BoardBar::BoardBar(QGraphicsItem* parent, qreal width, qreal height)
    : QGraphicsItem(parent),
      m_width(width),
      m_height(height)
{
    topHolder = new BarCheckerHolder(this, width, height * 0.3);
    bottomHolder = new BarCheckerHolder(this, width, height * 0.3);

    topHolder->setPos(0, 0.1*height);
    bottomHolder->setPos(0, 0.6*height);
}

QRectF BoardBar::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}

void BoardBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(boundingRect());

}
