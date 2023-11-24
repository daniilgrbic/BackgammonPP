#include "boardscene.h"

BoardScene::BoardScene(QObject *parent, qreal width, qreal height)
    : QGraphicsScene(parent),
      m_height(height),
      m_width(width),
      triangleHeight(m_height*0.4),
      triangleWidth(2.0*m_width/triangleNumber)

{

    this->setSceneRect(0, 0, m_width, m_height);
    this->setBoardTriangles();
}

void BoardScene::setBoardTriangles() {
    for(int i {0}; i < this->triangleNumber / 2; ++i){
        qreal x_point = this->triangleWidth * i;
        qreal y_point = m_height - triangleHeight;
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
