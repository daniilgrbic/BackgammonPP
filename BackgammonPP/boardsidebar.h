#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include "sidebarcheckerholder.h"

class BoardSideBar : public QGraphicsItem
{
public:
    BoardSideBar(QGraphicsItem* parent, qreal width, qreal height);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    SidebarCheckerHolder *topHolder, *bottomHolder;
private:
    qreal m_width;
    qreal m_height;
};
