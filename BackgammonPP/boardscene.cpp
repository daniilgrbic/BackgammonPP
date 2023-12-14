#include "boardscene.h"

BoardScene::BoardScene(QObject *parent, qreal width, qreal height)
    : QGraphicsScene(parent),
      m_height(height),
      m_width(width),
      triangleHeight(m_height*heightCoef), ///TODO: make constants instead of hardcoding
      triangleWidth( (m_width) / (trianglePairs + midBarCoef + 2 * sideBarCoef) ),
      barWidth(triangleWidth*midBarCoef),///TODO: here too
      sideBarWidth(triangleWidth*sideBarCoef),
      playingDieWidth(triangleWidth * playingDieCoef)
{

    setSceneRect(0, 0, m_width, m_height);
    setBoardTriangles();
    setBoardCheckers();
    setBoardBar();
    setSideBars();
    setPlayingDice();
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

void BoardScene::setPlayingDice()
{
    QMap<BoardPlayingDie::Position, QPointF> map1;
    QPointF Left1(sideBarWidth + 1.5*triangleWidth - 0.5*playingDieWidth
                  , 0.5*m_height - 0.5*playingDieWidth);
    QPointF Right1(sideBarWidth + 4.5*triangleWidth - 0.5*playingDieWidth
                  , 0.5*m_height - 0.5*playingDieWidth);
    map1[BoardPlayingDie::Position::LEFT] = std::move(Left1);
    map1[BoardPlayingDie::Position::RIGHT] = std::move(Right1);

    die1 = new BoardPlayingDie(nullptr, playingDieWidth, std::move(map1));

    QMap<BoardPlayingDie::Position, QPointF> map2;
    QPointF Left2(sideBarWidth + 6*triangleWidth + barWidth + 1.5*triangleWidth - 0.5*playingDieWidth
                  , 0.5*m_height - 0.5*playingDieWidth);
    QPointF Right2(sideBarWidth + 6*triangleWidth + barWidth + 4.5*triangleWidth - 0.5*playingDieWidth
                  , 0.5*m_height - 0.5*playingDieWidth);
    map2[BoardPlayingDie::Position::LEFT] = std::move(Left2);
    map2[BoardPlayingDie::Position::RIGHT] = std::move(Right2);

   die2 = new BoardPlayingDie(nullptr, playingDieWidth, std::move(map2));


    die1->hide();
    die2->hide();

    addItem(die1);
    addItem(die2);
}

void BoardScene::updatePlayingDice(int value1, int value2, BoardPlayingDie::Position pos){
    if(value1)
        die1->updateDie(pos, value1);
    if(value2)
        die2->updateDie(pos, value2);
}
