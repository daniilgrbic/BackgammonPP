#include "boardtriangle.h"
#include <QPainter>
#include <QBrush>

BoardTriangle::BoardTriangle(QGraphicsItem *parent, qreal x, qreal y, qreal width, qreal height, bool upwards)
    : QGraphicsItem(parent), m_x(x), m_y(y), m_width(width), m_height(height), m_upwards(upwards)
{
    if(m_upwards)
        m_polygon << QPointF(m_x, m_y + m_height) << QPointF(m_x + m_width, m_y + m_height) << QPointF(m_x + m_width / 2, m_y) << QPointF(m_x, m_y + m_height);
    else
        m_polygon << QPointF(m_x, m_y) << QPointF(m_x + m_width, m_y) << QPointF(m_x + m_width / 2, m_y + m_height) << QPointF(m_x, m_y);
}

QRectF BoardTriangle::boundingRect() const {
    return QRectF(m_x, m_y, m_width, m_height);
}

void BoardTriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::gray));
    painter->drawRect(boundingRect());

    painter->setBrush(QBrush(Qt::blue));
    painter->drawPolygon(m_polygon);

}
