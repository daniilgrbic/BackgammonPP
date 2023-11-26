#include "boardtriangle.h"
#include <QPainter>
#include <QBrush>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <iostream>
#include "boardchecker.h"

BoardTriangle::BoardTriangle(QGraphicsItem *parent, qreal x, qreal y, qreal width, qreal height, bool upwards)
    : QGraphicsItem(parent), CheckerHolder(), m_x(x), m_y(y), m_width(width), m_height(height), m_upwards(upwards)
{
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
    painter->drawRect(boundingRect());

    painter->setBrush(QBrush(Qt::blue));
    painter->drawPolygon(m_polygon);

}
void BoardTriangle::addChecker(BoardChecker *checker){
    if(checker->getHolder()){
        checker->getHolder()->removeChecker(checker);
        if(!checker->getHolder()->m_checkers.empty()){
            BoardChecker *lastChecker = checker->getHolder()->m_checkers.back();
            lastChecker->setFlag(QGraphicsItem::ItemIsSelectable, true);
            lastChecker->setFlag(QGraphicsItem::ItemIsMovable, true);
        }

    }
    checker->setHolder(this);
    if(!m_checkers.empty()){
        BoardChecker *lastChecker = m_checkers.back();
        lastChecker->setFlag(QGraphicsItem::ItemIsSelectable, false);
        lastChecker->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    m_checkers.push_back(checker);
    checker->setFlag(QGraphicsItem::ItemIsSelectable, true);
    checker->setFlag(QGraphicsItem::ItemIsMovable, true);
    if(m_upwards){
        checker->setPos(this->sceneBoundingRect().center().x(), this->pos().y() + m_height + checker->getSize() - 2 * checker->getSize() * m_checkers.size());
    }else{
        checker->setPos(this->sceneBoundingRect().center().x(),this->pos().y() - checker->getSize() + 2 * checker->getSize() * m_checkers.size());
    }
    checker->setAnchorPoint(checker->pos());



}

void BoardTriangle::removeChecker(BoardChecker *checker){
    m_checkers.erase(std::remove(m_checkers.begin(), m_checkers.end(), checker), m_checkers.end());
}
