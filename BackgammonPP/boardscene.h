#ifndef BOARDSCENE_H
#define BOARDSCENE_H

#include <QGraphicsScene>
#include <QVector>
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

    QVector<BoardTriangle*> boardTriangles;
    void setBoardTriangles();
};

#endif // BOARDSCENE_H
