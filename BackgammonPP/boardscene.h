#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QVector>
#include <QPainter>
#include <QPen>
#include "boardtriangle.h"

class BoardScene : public QGraphicsScene
{
public:
    BoardScene(QObject *parent);


private:
    const qreal m_height = 1.0;
    const qreal m_width = 1.0;
    const int triangleNumber = 24;
    const qreal triangleHeight = 0.4;
    const qreal triangleWidth = 1.0 / (qreal)this->triangleNumber;
    const QPainter m_painter;

    QVector<BoardTriangle*> boardTriangles;
    void setBoardTriangles();
    void drawBoardTriangle(BoardTriangle *boardTriangle);
};

#endif // BOARDSCENE_H
