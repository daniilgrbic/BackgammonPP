#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "checkerholder.h"

class BarCheckerHolder : public QGraphicsItem, public CheckerHolder
{
public:
    BarCheckerHolder(QGraphicsItem* parent, qreal width, qreal height);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    virtual void updateCheckerPos() override;
    virtual void updateHighlighting() override;
private:
    qreal m_width;
    qreal m_height;
};
