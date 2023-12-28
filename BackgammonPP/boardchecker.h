#pragma once

#include <QGraphicsItem>
#include <QObject>
#include "checkerholder.h"
#include "engine/core/playercolor.h"
#include "engine/core/specialposition.h"

class CheckerHolder;
using HolderType = std::variant<int, SpecialPosition>;

class BoardChecker :  public QGraphicsObject
{
    Q_OBJECT

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
    bool m_enabled = false;
    void setEnabledUpdate(bool enabled);

signals:
    void updateTurn(const HolderType origin, const HolderType to);
    void startMove(const HolderType origin);
    void endMove();

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
