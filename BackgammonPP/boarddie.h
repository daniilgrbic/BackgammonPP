#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include <QMap>

class BoardDie : public QGraphicsItem
{
public:
    BoardDie(QGraphicsItem *parent, qreal side);
    QRectF boundingRect() const override;
protected:
    qreal m_side;
    const QColor m_primary = Qt::black;
    const QColor m_secondary = Qt::white;
};
