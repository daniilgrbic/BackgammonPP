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
#include "boardplayingdie.h"
#include "boarddoublingdie.h"
#include "engine/core/boardstate.h"
#include "engine/core/turntrie.h"
#include "engine/core/roll.h"

class BoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    BoardScene(QObject *parent, qreal width, qreal height);
    void updatePlayingDice(int value1, int value2, BoardPlayingDie::Position pos);

    void setBoardState(const BoardState state);
    void getMoveInit(TurnTrie *trie);
    void getMoveUpdate(const HolderType origin, const HolderType to);
    void getMoveFinish();

    void setLegalTurns(std::vector<Turn> const *legalTurns);
    void setRoll(Roll const *roll);

    void prepareCheckers();
    void prepareHolders(const HolderType& origin);

signals:
    void enableEndTurn();

private:
    const qreal heightCoef = 0.4;
    const qreal midBarCoef = 1.0;
    const qreal sideBarCoef = 1.25;
    const qreal playingDieCoef = 0.9;
    const qreal doublingDieCoef = 0.7;
    const int trianglePairs = 12;
    const int checkersNumber = 30;

    const qreal m_height = 1.0;
    const qreal m_width = 1.0;
    const qreal triangleHeight = 100;
    const qreal triangleWidth = 50;
    const qreal barWidth = 1;
    const qreal sideBarWidth;
    const qreal playingDieSide;
    const qreal doublingDieSide;
    const QPainter m_painter;

    QVector<BoardTriangle*> boardTriangles;
    QVector<BoardChecker*> boardCheckers;
    QVector<BoardChecker*> whiteCheckers;
    QVector<BoardChecker*> blackCheckers;
    BoardPlayingDie *die1;
    BoardPlayingDie *die2;
    BoardDoublingDie *doublingDie;
    BoardBar *m_midBar;
    BoardSideBar *m_leftBar, *m_rightBar;
    CheckerHolder *whiteOut, *blackOut;
    CheckerHolder *whiteBar, *blackBar;

    TurnTrie* m_turnTrie;
    std::vector<Turn> const *legalTurns;
    Roll const *roll;


    void setBoardTriangles();
    void setBoardCheckers();
    void setBoardBar();
    void setSideBars();
    void setPlayingDice();
    void setDoublingDie();
    void drawBoardTriangle(BoardTriangle *boardTriangle);
    void disableAllCheckers();
    void disableAllHolders();

};

#endif // BOARDSCENE_H