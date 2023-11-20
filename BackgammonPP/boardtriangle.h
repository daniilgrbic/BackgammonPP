#ifndef BOARDTRIANGLE_H
#define BOARDTRIANGLE_H

#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QBrush>

class BoardTriangle : public QGraphicsPolygonItem
{
public:
    BoardTriangle(QGraphicsItem *parent, qreal m_width, qreal m_height);
    QPolygonF getPolygon();
private:
    qreal m_width;
    qreal m_height;
    bool m_upwards;
    QPolygonF m_polygon;
};

#endif // BOARDTRIANGLE_H
