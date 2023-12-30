#include "boardtriangle.h"
#include <QPainter>
#include <QBrush>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <iostream>
#include "boardchecker.h"

BoardTriangle::BoardTriangle(QGraphicsItem *parent, qreal x, qreal y, qreal width, qreal height, bool upwards, int number)
    : QGraphicsItem(parent), CheckerHolder(), m_x(x), m_y(y), m_width(width), m_height(height), m_upwards(upwards)
{
    m_type = number;
    setZValue(-1);
    this->setPos(m_x, m_y);
    if(m_upwards)
        m_polygon << QPointF(0, m_height) << QPointF(m_width, m_height) << QPointF(m_width / 2, 0) << QPointF(0, m_height);
    else
        m_polygon << QPointF(0, 0) << QPointF(m_width, 0) << QPointF(m_width / 2,m_height) << QPointF(0, 0);
}

QRectF BoardTriangle::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}


void BoardTriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::gray));

    if(canDropoff()){
        painter->setPen(QPen(Qt::red, 4));
    }

    painter->setBrush(QBrush(Qt::blue));
    painter->drawPolygon(m_polygon);

}

void BoardTriangle::updateCheckerPos()
{
    if(m_checkers.empty())
        return;
    qreal centerDistance = std::min((m_height - m_width) / (m_checkers.size() - 1), m_width);
    for(int i = 0; i < m_checkers.size(); i++){
        BoardChecker *checker = m_checkers[i];
        if(m_upwards){
            checker->setPos(this->sceneBoundingRect().center().x(), this->pos().y() + m_height - checker->getSize() - centerDistance * i);
        }else{
            checker->setPos(this->sceneBoundingRect().center().x(),this->pos().y() + checker->getSize() + centerDistance * i);
        }
        checker->setAnchorPoint(checker->pos());
        checker->setZValue(i);
    }
}

void BoardTriangle::updateHighlighting(){
    update();
}


