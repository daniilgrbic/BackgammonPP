#ifndef BOARDDOUBLINGDIE_H
#define BOARDDOUBLINGDIE_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPainter>
#include "boarddie.h"
#include <QVector>

class BoardDoublingDie : public BoardDie
{
public:
    BoardDoublingDie(QGraphicsItem *parent, qreal side, int value = 2);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void doubleValue();
private:
    int m_value;
    QVector<int> validValues = {2, 4, 8, 16, 32, 64};
};

#endif // BOARDDOUBLINGDIE_H
