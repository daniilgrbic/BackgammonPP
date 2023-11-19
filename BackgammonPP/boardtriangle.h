#ifndef BOARDTRIANGLE_H
#define BOARDTRIANGLE_H

#include <QGraphicsPolygonItem>

class BoardTriangle : public QGraphicsPolygonItem
{
public:
    BoardTriangle(QGraphicsItem *parent, qreal m_width, qreal m_height);
private:
    qreal m_width;
    qreal m_height;
    bool m_upwards;
};

#endif // BOARDTRIANGLE_H
