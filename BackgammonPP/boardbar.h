#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include "barcheckerholder.h"

class BoardBar : public QGraphicsItem
{
public:
    BoardBar(QGraphicsItem* parent, qreal width, qreal height);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    BarCheckerHolder *topHolder, *bottomHolder;

private:
    qreal m_width;
    qreal m_height;
};
