#include "verticalchecker.h"

VerticalChecker::VerticalChecker(QGraphicsItem* parent, qreal width, qreal height, QColor fill, QColor border)
    : QGraphicsItem(parent),
      m_width(width),
      m_height(height),
      m_fill(fill),
      m_border(border)
{
}

QRectF VerticalChecker::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}


void VerticalChecker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF drawRect = boundingRect();
    painter->setBrush(QBrush(m_fill));
    painter->setPen(QPen(m_border));
    painter->drawRoundedRect(drawRect, 4, 4);


}
