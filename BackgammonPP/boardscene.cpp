#include "boardscene.h"
#include <algorithm>
#include "boardwindow.h"

BoardScene::BoardScene(QObject *parent, qreal width, qreal height)
    : QGraphicsScene(parent),
      m_height(height),
      m_width(width),
      triangleHeight(m_height*heightCoef), ///TODO: make constants instead of hardcoding
      triangleWidth( (m_width) / (trianglePairs + midBarCoef + 2 * sideBarCoef) ),
      barWidth(triangleWidth*midBarCoef),///TODO: here too
      sideBarWidth(triangleWidth*sideBarCoef),
      playingDieSide(triangleWidth * playingDieCoef),
      doublingDieSide(doublingDieCoef*triangleWidth)
{

    setSceneRect(0, 0, m_width, m_height);
    setBoardTriangles();
    setBoardCheckers();
    setBoardBar();
    setSideBars();
    setPlayingDice();
    setDoublingDie();
    whiteOut = m_leftBar->bottomHolder;
    blackOut = m_leftBar->topHolder;
}

void BoardScene::setBoardTriangles() {
    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->sideBarWidth +
                this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = m_height - triangleHeight;
        BoardTriangle *bottomTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, true, trianglePairs - i);
        boardTriangles.push_back(bottomTriangle);
    }
    ///temporary fix for syncing triangle numerations between the board and the game engine, reverse the bottom row of triangles
    std::reverse(boardTriangles.begin(),boardTriangles.end());

    for(int i {0}; i < this->trianglePairs; ++i){
        qreal x_point = this->sideBarWidth +
                this->triangleWidth * i +
                ((i >= trianglePairs/2) ? barWidth : 0) ; //for triangles after the bar move their x coordinate
        qreal y_point = 0;
        BoardTriangle *upperTriangle = new BoardTriangle(nullptr, x_point, y_point, this->triangleWidth, this->triangleHeight, false, trianglePairs + 1 + i);
        boardTriangles.push_back(upperTriangle);
    }

    for(auto triangle : qAsConst(boardTriangles))
        this->drawBoardTriangle(triangle);


}
void BoardScene::setBoardCheckers(){


    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, triangleWidth / 2, PlayerColor::BLACK);
        boardCheckers.push_back(checker);
        blackCheckers.push_back(checker);
    }
    for(int i = 0; i < checkersNumber / 2; ++i){
        BoardChecker *checker = new BoardChecker(nullptr, triangleWidth / 2, PlayerColor::WHITE);
        boardCheckers.push_back(checker);
        whiteCheckers.push_back(checker);
    }
    for(auto checker : qAsConst(boardCheckers))
        this->addItem(checker);

    int i = 0;
    int j = 0;
    /*
    for(int n = i + 5; i < n; ++i)
        boardTriangles[11]->addChecker(blackCheckers[i]);

    for(int n = j + 3; j < n; ++j)
        boardTriangles[7]->addChecker(whiteCheckers[j]);

    for(int n = j + 5; j < n; ++j)
        boardTriangles[5]->addChecker(whiteCheckers[j]);

    for(int n = i + 2; i < n; ++i)
        boardTriangles[0]->addChecker(blackCheckers[i]);

    for(int n = j + 5; j < n; ++j)
        boardTriangles[12]->addChecker(whiteCheckers[j]);

    for(int n = i + 3; i < n; ++i)
        boardTriangles[16]->addChecker(blackCheckers[i]);

    for(int n = i + 5; i < n; ++i)
        boardTriangles[18]->addChecker(blackCheckers[i]);

    for(int n = j + 2; j < n; ++j)
        boardTriangles[23]->addChecker(whiteCheckers[j]);
        */
}

void BoardScene::setBoardBar()
{
   m_midBar = new BoardBar(nullptr, triangleWidth, m_height);
   m_midBar->setPos(this->sideBarWidth + (trianglePairs/2) * triangleWidth, 0);
   addItem(m_midBar);
   whiteBar = m_midBar->topHolder;
   blackBar = m_midBar->bottomHolder;
}

void BoardScene::setSideBars()
{
    m_leftBar = new BoardSideBar(nullptr, sideBarWidth, m_height);
    m_leftBar->setPos(0, 0);
    addItem(m_leftBar);

    m_rightBar = new BoardSideBar(nullptr, sideBarWidth, m_height);
    m_rightBar->setPos(m_width-sideBarWidth, 0);
    addItem(m_rightBar);
}


void BoardScene::drawBoardTriangle(BoardTriangle *boardTriangle) {
    this->addItem(boardTriangle);
}

void BoardScene::setPlayingDice()
{
    QMap<BoardPlayingDie::Position, QPointF> map1;
    QPointF Left1(sideBarWidth + 1.5*triangleWidth - 0.5*playingDieSide
                  , 0.5*m_height - 0.5*playingDieSide);
    QPointF Right1(sideBarWidth + 6*triangleWidth + barWidth + 1.5*triangleWidth - 0.5*playingDieSide
                  , 0.5*m_height - 0.5*playingDieSide);
    map1[BoardPlayingDie::Position::LEFT] = std::move(Left1);
    map1[BoardPlayingDie::Position::RIGHT] = std::move(Right1);

    die1 = new BoardPlayingDie(nullptr, playingDieSide, std::move(map1));

    QMap<BoardPlayingDie::Position, QPointF> map2;
    QPointF Left2(sideBarWidth + 4.5*triangleWidth - 0.5*playingDieSide
                  , 0.5*m_height - 0.5*playingDieSide);
    QPointF Right2(sideBarWidth + 6*triangleWidth + barWidth + 4.5*triangleWidth - 0.5*playingDieSide
                  , 0.5*m_height - 0.5*playingDieSide);
    map2[BoardPlayingDie::Position::LEFT] = std::move(Left2);
    map2[BoardPlayingDie::Position::RIGHT] = std::move(Right2);

   die2 = new BoardPlayingDie(nullptr, playingDieSide, std::move(map2));


    die1->hide();
    die2->hide();

    addItem(die1);
    addItem(die2);
}

void BoardScene::setDoublingDie(){
    QMap<BoardDoublingDie::Position, QPointF> map;
    QPointF Out( (sideBarWidth - doublingDieSide) * 0.5
                  , 0.5*m_height - 0.5*doublingDieSide);

    QPointF Left(sideBarWidth + (trianglePairs * 0.25) *triangleWidth - 0.5*doublingDieSide
                  , 0.5*m_height - 0.5*doublingDieSide);
    QPointF Right(sideBarWidth + (trianglePairs * 0.5) * triangleWidth + barWidth + (trianglePairs * 0.25)*triangleWidth - 0.5*doublingDieSide
                  , 0.5*m_height - 0.5*doublingDieSide);

    map[BoardDoublingDie::Position::OUT] = std::move(Out);
    map[BoardDoublingDie::Position::LEFT] = std::move(Left);
    map[BoardDoublingDie::Position::RIGHT] = std::move(Right);

    doublingDie = new BoardDoublingDie(nullptr, doublingDieSide, std::move(map));
    doublingDie->updateDie(BoardDoublingDie::Position::OUT, 2);
    addItem(doublingDie);
}

void BoardScene::updatePlayingDice(int value1, int value2, BoardPlayingDie::Position pos){
    if(value1)
        die1->updateDie(pos, value1);
    if(value2)
        die2->updateDie(pos, value2);
}

void BoardScene::disableAllCheckers(){
    for(BoardChecker *checker: boardCheckers){
        checker->setFlag(QGraphicsItem::ItemIsSelectable, false);
        checker->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void BoardScene::disableAllHolders(){
    for(BoardTriangle *triangle: boardTriangles){
        triangle->allowDropoff = false;
    }
    m_leftBar->topHolder->allowDropoff = false;
    m_leftBar->topHolder->allowDropoff = false;
    m_rightBar->bottomHolder->allowDropoff = false;
    m_rightBar->bottomHolder->allowDropoff = false;

}


void BoardScene::setBoardState(const BoardState state){
    int blackCheckerCount = 0, whiteCheckerCount = 0;
    assert(NUMBER_OF_POINTS == trianglePairs * 2);
    for(int i = 1; i <= NUMBER_OF_POINTS; i++){
        auto point = state.point(i);
        if(point.owner() == PlayerColor::WHITE){
            for(int j = 0; j < point.count(); j++){
                boardTriangles[i-1]->addChecker(whiteCheckers[whiteCheckerCount]);
                whiteCheckerCount++;
            }
        }else{
            for(int j = 0; j < point.count(); j++){
                boardTriangles[i-1]->addChecker(blackCheckers[blackCheckerCount]);
                blackCheckerCount++;
            }
        }
    }
    for(int j = 0; j < state.bar(PlayerColor::WHITE); j++){
        m_midBar->topHolder->addChecker(whiteCheckers[whiteCheckerCount]);
        whiteCheckerCount++;
    }

    for(int j = 0; j < state.bar(PlayerColor::BLACK); j++){
        m_midBar->bottomHolder->addChecker(blackCheckers[blackCheckerCount]);
        blackCheckerCount++;
    }

    for(int j = 0; j < state.off(PlayerColor::WHITE); j++){
        whiteOut->addChecker(whiteCheckers[whiteCheckerCount]);
        whiteCheckerCount++;
    }

    for(int j = 0; j < state.off(PlayerColor::BLACK); j++){
        blackOut->addChecker(blackCheckers[blackCheckerCount]);
        blackCheckerCount++;
    }

    assert(whiteCheckerCount == whiteCheckers.size() && blackCheckerCount == blackCheckers.size());
}

void BoardScene::getMoveInit(TurnTrie *trie){
    this->m_turnTrie = trie;
    prepareCheckers();
}

void BoardScene::prepareCheckers(){
    std::vector<Move> nextMoves = m_turnTrie->nextMoves();
    for(Move &move : nextMoves){
        HolderType fromType = move.m_from;
        if(const SpecialPosition *specPos = std::get_if<SpecialPosition>(&fromType)){
            assert(*specPos == SpecialPosition::BAR);
            if(move.m_player == PlayerColor::WHITE){
                whiteBar->enableCheckers(PlayerColor::WHITE);
            }else{
                blackBar->enableCheckers(PlayerColor::BLACK);
            }
        }else if(const int *point = std::get_if<int>(&fromType)){
            assert((*point) >= 1 && (*point <= 24));
            boardTriangles[(*point) - 1]->enableCheckers(move.m_player);
        }
    }
}

void BoardScene::prepareHolders(const HolderType &origin){
    std::vector<Move> nextMoves = m_turnTrie->nextMoves();
    for(const Move& move : nextMoves){
        if(move.m_from != origin){
            continue;
        }
        if(const SpecialPosition *specPos = std::get_if<SpecialPosition>(&move.m_to)){
            if(move.m_player == PlayerColor::WHITE){
                if(*specPos == SpecialPosition::BAR){
                    whiteBar->allowDropoff = true;
                }else if(*specPos == SpecialPosition::OFF){
                    whiteOut->allowDropoff = true;
                }
            }else if(move.m_player == PlayerColor::BLACK){
                if(*specPos == SpecialPosition::BAR){
                    blackBar->allowDropoff = true;
                }else if(*specPos == SpecialPosition::OFF){
                    blackOut->allowDropoff = true;
                }
            }
        }else if(const int *point = std::get_if<int>(&move.m_to)){
            assert((*point) >= 1 && (*point <= 24));
            boardTriangles[(*point) - 1]->allowDropoff = true;
        }

    }
}

void BoardScene::getMoveUpdate(const HolderType origin, const HolderType to){
    std::vector<Move> nextMoves = m_turnTrie->nextMoves();
    Move const *nextMove = nullptr;
    for(const Move &move : nextMoves){
        if(move.m_from == origin && move.m_to == to){
            m_turnTrie->playMove(move);
        }
    }
    if(m_turnTrie->isFinishedTurn()){
        emit enableEndTurn();
    }
}

void BoardScene::setLegalTurns(std::vector<Turn> const *legalTurns){
    this->legalTurns = legalTurns;
}

void BoardScene::setRoll(Roll const *roll){
    this->roll = roll;
}