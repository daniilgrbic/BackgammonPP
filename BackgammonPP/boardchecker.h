#ifndef BOARDCHECKER_H
#define BOARDCHECKER_H

#include <QGraphicsItem>
#include "boardtriangle.h"

class BoardTriangle;

class BoardChecker : public QGraphicsItem
{
public:
    BoardChecker(QGraphicsItem *parent, qreal size, QColor color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void setAnchorPoint(const QPointF& anchorPoint);
    void setTriangle(BoardTriangle* triangle);
    qreal getSize();
    BoardTriangle *getTriangle();
private:
    qreal m_size;
    QColor m_color;
    QPointF m_anchorPoint;
    bool m_dragged;
    BoardTriangle* m_triangle;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // BOARDCHECKER_H
