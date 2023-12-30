#pragma once

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
#include "consts.h"

class BoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    BoardScene(QObject *parent, qreal width, qreal height);
    ~BoardScene();

    void updatePlayingDice();
    void updatePlayingDice(const Roll& roll);

    void setBoardState(const BoardState &state);
    void getTurnInit();
    void getTurnFinish();

    void setLegalTurns(std::vector<Turn> const *legalTurns);
    void setRoll(Roll const *roll);
    const Roll* getRoll();

    void prepareCheckers();
    void prepareHolders(const HolderType origin);

    void undoMove();
    void setExitPoints(GameType);

signals:
    void enableEndTurn();
    void setUndoEnabled(bool enable);
    void sendTurnFinish(Turn);

public slots:
    void checkerStartMoving(const HolderType origin);
    void checkerEndMoving();
    void getTurnUpdate(const HolderType origin, const HolderType to);

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

    BoardState state;

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

    TurnTrie* m_turnTrie { nullptr };
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
