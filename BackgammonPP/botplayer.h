#pragma once

#include <QObject>
#include "player.h"

class BotPlayer : public Player
{
    Q_OBJECT
public:
    explicit BotPlayer(QObject *parent = nullptr);

public slots:
    //connected to the match
    //void chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll) override;
    //void setState(const BoardState& state) override;
    //void setDice(const Roll& roll) override;

    //connected to bot
    //void acceptMove(Turn turn) override;
    //void diceRolled(Roll roll) override;
};
