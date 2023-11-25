#include "boardscene.h"

BoardScene::BoardScene(QObject *parent, qreal width, qreal height)
    : QGraphicsScene(parent),
      m_height(height),
      m_width(width),
      barWidth(m_width / (1 + trianglePairs) ),///TODO: here too
      triangleHeight(m_height*0.4), ///TODO: make constants instead of hardcoding
      triangleWidth((m_width-barWidth)/trianglePairs)
{

    this->setSceneRect(0, 0, m_width, m_height);
    this->setBoardTriangles();
    this->setBoardCheckers();
}

void BoardScene::setBoardTriangles() {
    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = m_height - triangleHeight;
        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, true);
        boardTriangles.push_back(bottomTriangle);
    }
    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = 0;
        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, false);
        boardTriangles.push_back(upperTriangle);
    }

    for(auto triangle : qAsConst(boardTriangles))
        this->drawBoardTriangle(triangle);


}
void BoardScene::setBoardCheckers(){
    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, 25, Qt::black);
        boardCheckers.push_back(checker);
        blackCheckers.push_back(checker);
    }
    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, 25, Qt::white);
        boardCheckers.push_back(checker);
        whiteCheckers.push_back(checker);
    }
    for(auto checker : qAsConst(boardCheckers))
        this->addItem(checker);

    int i = 0;
    int j = 0;
    for(int n = i + 5; i < n; ++i)
        boardTriangles[0]->addChecker(blackCheckers[i]);

    for(int n = j + 3; j < n; ++j)
        boardTriangles[4]->addChecker(whiteCheckers[j]);

    for(int n = j + 5; j < n; ++j)
        boardTriangles[6]->addChecker(whiteCheckers[j]);

    for(int n = i + 2; i < n; ++i)
        boardTriangles[11]->addChecker(blackCheckers[i]);

    for(int n = j + 5; j < n; ++j)
        boardTriangles[12]->addChecker(whiteCheckers[j]);

    for(int n = i + 3; i < n; ++i)
        boardTriangles[16]->addChecker(blackCheckers[i]);

    for(int n = i + 5; i < n; ++i)
        boardTriangles[18]->addChecker(blackCheckers[i]);

    for(int n = j + 2; j < n; ++j)
        boardTriangles[23]->addChecker(whiteCheckers[j]);
}

void BoardScene::drawBoardTriangle(BoardTriangle *boardTriangle) {
    this->addItem(boardTriangle);
}
