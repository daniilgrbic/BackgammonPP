#include "boarddoublingdie.h"

BoardDoublingDie::BoardDoublingDie(QGraphicsItem *parent, qreal side, int value)
    : BoardDie(parent, side),
      m_value(value)
{
    assert(validValues.contains(value));
}


void BoardDoublingDie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF drawRect = boundingRect();
    painter->setBrush(QBrush(m_secondary));
    painter->setPen(QPen(m_primary, 0.07 * m_side));
    painter->drawRect(drawRect);

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSizeF(0.5 * m_side);
    painter->setBrush(QBrush(m_primary));
    painter->setFont(font);
    painter->drawText(drawRect, Qt::AlignCenter, QString::number(m_value));
}

void BoardDoublingDie::doubleValue(){
    m_value *= 2;
    assert(validValues.contains(m_value));
    update();
}

