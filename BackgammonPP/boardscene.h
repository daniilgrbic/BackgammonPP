#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>
#include <QRectF>
#include <QVector>
#include <QPainter>
#include <QPen>
#include "boardtriangle.h"
#include "boardchecker.h"
#include "boardbar.h"
#include "boardsidebar.h"
#include "boarddice.h"

class BoardScene : public QGraphicsScene
{
public:
    BoardScene(QObject *parent, qreal width, qreal height);


private:
    const qreal heightCoef = 0.4;
    const qreal midBarCoef = 1.0;
    const qreal sideBarCoef = 1.25;

    const qreal m_height = 1.0;
    const qreal m_width = 1.0;
    const int trianglePairs = 12;
    const int checkersNumber = 30;
    const qreal triangleHeight = 100;
    const qreal triangleWidth = 50;
    const qreal barWidth = 1;
    const qreal sideBarWidth;
    const QPainter m_painter;

    QVector<BoardTriangle*> boardTriangles;
    QVector<BoardChecker*> boardCheckers;
    QVector<BoardChecker*> whiteCheckers;
    QVector<BoardChecker*> blackCheckers;
    void setBoardTriangles();
    void setBoardCheckers();
    void setBoardBar();
    void setSideBars();
    void drawBoardTriangle(BoardTriangle *boardTriangle);
};

#endif // BOARDSCENE_H
