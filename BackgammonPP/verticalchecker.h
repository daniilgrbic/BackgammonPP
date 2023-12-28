#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "checkerholder.h"

class VerticalChecker : public QGraphicsItem
{
public:
    VerticalChecker(QGraphicsItem* parent, qreal width, qreal height, QColor fill, QColor border);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    qreal m_width;
    qreal m_height;
    QColor m_fill;
    QColor m_border;

};
