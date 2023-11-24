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
    BoardScene(QObject *parent, qreal width, qreal height);


private:
    const qreal m_height = 1.0;
    const qreal m_width = 1.0;
    const int trianglePairs = 12;
    const qreal barWidth = 1;
    const qreal triangleHeight = 100;
    const qreal triangleWidth = 50;
    const QPainter m_painter;

    QVector<BoardTriangle*> boardTriangles;
    void setBoardTriangles();
    void drawBoardTriangle(BoardTriangle *boardTriangle);
};

#endif // BOARDSCENE_H
