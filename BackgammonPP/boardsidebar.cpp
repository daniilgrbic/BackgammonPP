#include "boardsidebar.h"
BoardSideBar::BoardSideBar(QGraphicsItem* parent, qreal width, qreal height)
    : QGraphicsItem(parent),
      m_width(width),
      m_height(height)
{
    qreal holderWidth = m_width / 1.25;
    qreal holderHeight = m_height * 0.33;
    qreal holderX = (m_width - holderWidth) / 2;
    topHolder = new SidebarCheckerHolder(this, holderWidth, holderHeight);
    topHolder->setPos(holderX, (m_height/2 - holderHeight) / 2);
    bottomHolder = new SidebarCheckerHolder(this, holderWidth, holderHeight);
    bottomHolder->setPos(holderX, m_height/2 + (m_height/2 - holderHeight) / 2);
}

QRectF BoardSideBar::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}

void BoardSideBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(boundingRect());

}
