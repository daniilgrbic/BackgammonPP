#include "barcheckerholder.h"

BarCheckerHolder::BarCheckerHolder(QGraphicsItem *parent, qreal width, qreal height)
    : QGraphicsItem(parent),
      CheckerHolder(),
      m_width(width),
      m_height(height)
{
    m_type = SpecialPosition::BAR;
}

QRectF BarCheckerHolder::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}


void BarCheckerHolder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::red));
    painter->setOpacity(0.05);
    painter->drawRect(boundingRect());
}

void BarCheckerHolder::updateCheckerPos()
{
    if(m_checkers.empty())
        return;
    qreal centerDistance = m_width / 8;
    for(int i = 0; i < m_checkers.size(); i++){
        BoardChecker *checker = m_checkers[i];
        checker->setPos(this->sceneBoundingRect().center().x(),this->pos().y() + checker->getSize() + centerDistance * i);
        checker->setAnchorPoint(checker->pos());
        checker->setZValue(i+1);
    }
}

void BarCheckerHolder::updateHighlighting(){

}
