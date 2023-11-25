#ifndef BOARDTRIANGLE_H
#define BOARDTRIANGLE_H

#include <QGraphicsItem>
#include <QPolygonF>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include "boardchecker.h"
#include <QVector>

class BoardChecker;

class BoardTriangle : public QGraphicsItem
{
public:
    BoardTriangle(QGraphicsItem *parent, qreal m_x, qreal m_y, qreal width, qreal height, bool upwards);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void addChecker(BoardChecker* checker);
    void removeChecker(BoardChecker* checker);
private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    bool m_upwards;
    QPolygonF m_polygon;

    int m_checkersNumber;
    QVector<BoardChecker*> m_checkers;
};

#endif // BOARDTRIANGLE_H
