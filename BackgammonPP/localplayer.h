#pragma once

#include <QObject>
#include "playableplayer.h"
#include "boardwindow.h"

class LocalPlayer : public QObject // : public PlayablePlayer
{
    Q_OBJECT
public:
    explicit LocalPlayer(QObject *parent, BoardWindow *m_board);
signals:
    void returnAcquiredTurn(Turn);

signals:
    //connected to the match
    void returnMove(Turn turn);
    void confirmRoll();

    //connected to the board
    void forwardMoveRequest(std::vector<Turn> *legalMoves, Roll *roll); //override;
    void forwardSetState(const BoardState &state);
    void forwardSetDice(const Roll &roll);

public slots:
    //connected to the match
    void chooseMove(std::vector<Turn> *legalMoves, Roll *roll); //override;
    void setState(const BoardState &state); //override;
    void setDice(const Roll &roll);

    //connected to the board
    void acceptMove(Turn turn);
    void diceRolled();

private:
    std::vector<Turn> *m_legalMoves;
    Roll *m_roll;
    BoardWindow *m_board;
};


