#include "boardchecker.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QCursor>
#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QDrag>
#include <QWidget>
#include <QList>
#include <QLineF>
#include "checkerholder.h"

BoardChecker::BoardChecker(QGraphicsItem *parent, qreal size, QColor color) : QGraphicsItem(parent) , m_size(size), m_color(color), m_dragged(false), m_holder(nullptr)
{
    setZValue(1);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCursor(Qt::OpenHandCursor);
}
QRectF BoardChecker::boundingRect() const {
    return QRectF(-m_size,-m_size, 2 * m_size, 2 * m_size);
}

void BoardChecker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
//    painter->setBrush(QBrush(Qt::gray));
//    painter->drawRect(boundingRect());

    painter->setBrush(QBrush(m_color));
    painter->drawEllipse(QPoint(0,0),m_size,m_size);
}

void BoardChecker::setAnchorPoint(const QPointF &anchorPoint){
    this->m_anchorPoint = anchorPoint;
}

void BoardChecker::setHolder(CheckerHolder *holder){
    m_holder = holder;
}
qreal BoardChecker::getSize(){
    return m_size;
}
CheckerHolder *BoardChecker::getHolder(){
    return m_holder;
}


void BoardChecker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    setZValue(100);
    QGraphicsItem::mousePressEvent(event);
}
void BoardChecker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
        setZValue(1);
    setCursor(Qt::OpenHandCursor);
    if(m_dragged){
        QList<QGraphicsItem*> colItems = collidingItems();
        if(colItems.empty())
            this->setPos(m_anchorPoint);
        else {
            QGraphicsItem * closestItem = colItems.at(0);
            qreal shortestDist = 100000;
            for(QGraphicsItem* item : qAsConst(colItems)){
                QLineF line(item->sceneBoundingRect().center(), this->sceneBoundingRect().center());
                CheckerHolder* itemAsHolder = dynamic_cast<CheckerHolder*>(item);
                if(itemAsHolder && itemAsHolder->allowDropoff && line.length() < shortestDist){
                    shortestDist = line.length();
                    closestItem = item;
                }
            }
            if(CheckerHolder *closestHolder = dynamic_cast<CheckerHolder*>(closestItem)){
                    closestHolder->addChecker(this);
            }else{
                this->setPos(m_anchorPoint);
            }

        }
        m_dragged = false;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void BoardChecker::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
//    std::cout << "Move" << std::endl;
    setCursor(Qt::OpenHandCursor);
    m_dragged = true;
    QGraphicsItem::mouseMoveEvent(event);

}
