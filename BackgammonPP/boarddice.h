#ifndef BOARDDICE_H
#define BOARDDICE_H
#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include <QVector>

class BoardDice : public QGraphicsItem
{
public:
    BoardDice(QGraphicsItem *parent, qreal side, int value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
private:
    qreal m_side;
    qreal m_dotSizeShare = 0.15;
    int m_value;
    qreal m_dotDiameter;
    //MOVE OUT TO WHEREVER CONSTANTS ARE.
    const QColor m_primary = Qt::black;
    const QColor m_secondary = Qt::white;

    const QVector<QPointF> dotPositions[6] = {
        { QPointF(0.50, 0.50) },
        { QPointF(0.25, 0.25), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.50, 0.50), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75), QPointF(0.50, 0.50) },
        { QPointF(0.25, 0.25), QPointF(0.25, 0.75), QPointF(0.75, 0.25), QPointF(0.75, 0.75), QPointF(0.25, 0.50), QPointF(0.75, 0.50) },
    };
};

#endif // BOARDDICE_H
