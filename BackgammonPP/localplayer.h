#pragma once

#include <QObject>
#include "player.h"
#include "boardwindow.h"

class LocalPlayer : public Player
{
    Q_OBJECT
public:
    explicit LocalPlayer(QObject *parent, BoardWindow *m_board);

signals:
    //connected to the board
    void forwardMoveRequest(std::vector<Turn> *legalMoves, Roll *roll);

public slots:
    //connected to the match
    void chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll) override;
    void setState(const BoardState& state) override;
    void setDice(const Roll& roll) override;

    //connected to the board
    void acceptMove(Turn turn) override;
    void diceRolled(Roll roll) override;

private:
    BoardWindow *m_board;

    void connectToBoard();
    void disconnectFromBoard();
};


