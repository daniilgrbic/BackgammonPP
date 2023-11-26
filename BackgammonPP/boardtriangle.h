#ifndef BOARDTRIANGLE_H
#define BOARDTRIANGLE_H

#include <QGraphicsItem>
#include <QPolygonF>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include "boardchecker.h"
#include "checkerholder.h"
#include <QVector>

class BoardChecker;

class BoardTriangle : public QGraphicsItem, public CheckerHolder
{
public:
    BoardTriangle(QGraphicsItem *parent, qreal m_x, qreal m_y, qreal width, qreal height, bool upwards);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    virtual void addChecker(BoardChecker* checker) override;
    virtual void removeChecker(BoardChecker* checker) override;
private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    bool m_upwards;
    QPolygonF m_polygon;

};

#endif // BOARDTRIANGLE_H
