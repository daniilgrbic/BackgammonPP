#include "boardscene.h"

BoardScene::BoardScene(QObject *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(0,0,600,300);
    this->setBoardTriangles();
}

void BoardScene::setBoardTriangles() {
    for(int i {0}; i < this->triangleNumber / 2; ++i){
        qreal x_point = this->triangleWidth * i;
        qreal y_point = 200;
        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, true);
        boardTriangles.push_back(bottomTriangle);
    }
    for(int i {0}; i < this->triangleNumber / 2; ++i){
        qreal x_point = this->triangleWidth * i;
        qreal y_point = 0;
        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, false);
        boardTriangles.push_back(upperTriangle);
    }

    for(auto triangle : qAsConst(boardTriangles))
        this->drawBoardTriangle(triangle);
}

void BoardScene::drawBoardTriangle(BoardTriangle *boardTriangle) {
    this->addItem(boardTriangle);
}
