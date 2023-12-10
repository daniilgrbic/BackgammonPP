#include "boardscene.h"

BoardScene::BoardScene(QObject *parent, qreal width, qreal height)
    : QGraphicsScene(parent),
      m_height(height),
      m_width(width),
      triangleHeight(m_height*heightCoef), ///TODO: make constants instead of hardcoding
      triangleWidth( (m_width) / (trianglePairs + midBarCoef + 2 * sideBarCoef) ),
      barWidth(triangleWidth*midBarCoef),///TODO: here too
      sideBarWidth(triangleWidth*sideBarCoef)
{

    this->setSceneRect(0, 0, m_width, m_height);
    this->setBoardTriangles();
    this->setBoardCheckers();
    setBoardBar();
    setSideBars();
}

void BoardScene::setBoardTriangles() {
    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->sideBarWidth +
                this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = m_height - triangleHeight;
        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, true);
        boardTriangles.push_back(bottomTriangle);
    }
    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->sideBarWidth +
                this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = 0;
        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, false);
        boardTriangles.push_back(upperTriangle);
    }

    for(auto triangle : qAsConst(boardTriangles))
        this->drawBoardTriangle(triangle);


}
void BoardScene::setBoardCheckers(){

    ///TODO: remove this.
    BoardDice *die = new BoardDice(nullptr, triangleWidth, 6);
    addItem(die);
    die->setPos(0.25*m_width, 0.45*m_height);

    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, triangleWidth / 2, Qt::black);
        boardCheckers.push_back(checker);
        blackCheckers.push_back(checker);
    }
    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, triangleWidth / 2, Qt::white);
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

void BoardScene::setBoardBar()
{
   BoardBar *bar = new BoardBar(nullptr, triangleWidth, m_height);
   bar->setPos(this->sideBarWidth + (trianglePairs/2) * triangleWidth, 0);
   addItem(bar);
}

void BoardScene::setSideBars()
{
    BoardSideBar *left = new BoardSideBar(nullptr, sideBarWidth, m_height);
    left->setPos(0, 0);
    addItem(left);

    BoardSideBar *right = new BoardSideBar(nullptr, sideBarWidth, m_height);
    right->setPos(m_width-sideBarWidth, 0);
    addItem(right);
}


void BoardScene::drawBoardTriangle(BoardTriangle *boardTriangle) {
    this->addItem(boardTriangle);
}


