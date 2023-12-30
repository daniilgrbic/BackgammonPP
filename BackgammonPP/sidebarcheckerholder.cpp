#include "sidebarcheckerholder.h"
#include <iostream>

SidebarCheckerHolder::SidebarCheckerHolder(QGraphicsItem *parent, qreal width, qreal height)
    : QGraphicsItem(parent),
      CheckerHolder(),
      m_width(width),
      m_height(height)
{
    setZValue(1);
    m_type = SpecialPosition::OFF;
}

QRectF SidebarCheckerHolder::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}


void SidebarCheckerHolder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::gray));
    if(canDropoff()){
        painter->setPen(QPen(Qt::red, 4));
    }
    painter->drawRect(boundingRect());

}

void SidebarCheckerHolder::updateCheckerPos()
{
    for(int i = 0; i < m_checkers.size(); i++){
        BoardChecker *checker = m_checkers[i];
        checker->setPos(this->sceneBoundingRect().center().x(),this->pos().y() + checker->getSize());
        checker->setAnchorPoint(checker->pos());
        checker->setZValue(-1);
    }
    updateVericalCheckers();
}

void SidebarCheckerHolder::updateVericalCheckers()
{
    while(verticalCheckers.size() > m_checkers.size()){
        VerticalChecker* back = verticalCheckers.back();
        verticalCheckers.pop_back();
        delete back;
    }

    while(verticalCheckers.size() < m_checkers.size()){
        int checkerNumber = 15;
        qreal VCheckerWidth = m_width;
        qreal VCheckerHeight = m_height / checkerNumber;
        uint index = verticalCheckers.size();
        VerticalChecker* newChecker = new VerticalChecker(this, VCheckerWidth, VCheckerHeight, m_checkers.back()->getQColor(), Qt::gray);
        newChecker->setPos(0, index * VCheckerHeight);
        newChecker->setZValue(2);
        verticalCheckers.push_back(newChecker);


    }

}

void SidebarCheckerHolder::updateHighlighting(){
    update();
}
