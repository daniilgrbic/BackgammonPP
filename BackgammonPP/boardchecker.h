#ifndef BOARDCHECKER_H
#define BOARDCHECKER_H

#include <QGraphicsItem>
#include "checkerholder.h"
#include "engine/core/playercolor.h"

class CheckerHolder;

class BoardChecker : public QGraphicsItem
{
public:
    BoardChecker(QGraphicsItem *parent, qreal size, PlayerColor color);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void setAnchorPoint(const QPointF& anchorPoint);
    void setHolder(CheckerHolder* holder);
    qreal getSize();
    CheckerHolder *getHolder();
    const PlayerColor getColor() const;
    const QColor getQColor() const;

private:
    qreal m_size;
    PlayerColor m_color;
    QColor m_QColor;
    QPointF m_anchorPoint;
    bool m_dragged;
    CheckerHolder* m_holder;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // BOARDCHECKER_H
