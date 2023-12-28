#pragma once

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
    BoardTriangle(QGraphicsItem *parent, qreal m_x, qreal m_y, qreal width, qreal height, bool upwards, int number);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void updateCheckerPos() override;
    void updateHighlighting() override;
private:
    qreal m_x;
    qreal m_y;
    qreal m_width;
    qreal m_height;
    bool m_upwards;
    QPolygonF m_polygon;
};
