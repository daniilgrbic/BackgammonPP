#pragma once

#include <QObject>
#include <QString>
#include <vector>
#include "engine/core/boardstate.h"
#include "engine/core/playercolor.h"
#include "engine/core/turn.h"
#include "engine/core/roll.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

signals:
    //connected to the match
    void returnMove(Turn turn); // salje od playera (gui) -> match odigrani potez
    void confirmRoll(Roll roll); //

    //connected to the board/client/bot
    void forwardSetState(const BoardState &state);
    void forwardSetDice(const Roll &roll);

public slots:
    //connected to the match
    virtual void setState(const BoardState& state) = 0;
    virtual void setDice(const Roll& roll) = 0;
    virtual void chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll) = 0;

    //connected to board/client/bot
    virtual void acceptMove(Turn turn) = 0;
    virtual void diceRolled(Roll roll) = 0;

private:
    std::vector<Turn> *m_legalMoves;
    Roll *m_roll;

    //Client* m_client; -> u remote player
    PlayerColor m_playerColor;
};
