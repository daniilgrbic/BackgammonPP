#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "checkerholder.h"
#include "verticalchecker.h"
#include <QVector>
class SidebarCheckerHolder : public QGraphicsItem, public CheckerHolder
{
public:
    SidebarCheckerHolder(QGraphicsItem* parent, qreal width, qreal height);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    virtual void updateCheckerPos() override;
    virtual void updateHighlighting() override;
private:
    qreal m_width;
    qreal m_height;
    QVector<VerticalChecker *> verticalCheckers;
    void updateVericalCheckers();
};
