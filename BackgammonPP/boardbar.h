#ifndef BOARDBAR_H
#define BOARDBAR_H

#include <QGraphicsItem>
#include <QPainter>

class BoardBar : public QGraphicsItem
{
public:
    BoardBar(QGraphicsItem* parent, qreal width, qreal height);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

private:
    qreal m_width;
    qreal m_height;

};

#endif // BOARDBAR_H
