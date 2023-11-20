#include "boardtriangle.h"

BoardTriangle::BoardTriangle(QGraphicsItem *parent, qreal m_width, qreal m_height)
    : QGraphicsPolygonItem(parent), m_width(m_width), m_height(m_height)
{
    m_polygon << QPointF(0, 0) << QPointF(100, 0) << QPointF(100, 100);

    this->setPolygon(m_polygon);
    this->setBrush(QBrush(Qt::red, Qt::SolidPattern));
}

QPolygonF BoardTriangle::getPolygon() {
    return this->m_polygon;
}
