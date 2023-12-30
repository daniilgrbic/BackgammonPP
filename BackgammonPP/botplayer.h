#pragma once

#include <QObject>
#include "player.h"
#include "engine/bot/bot.hpp"
#include "boardwindow.h"
#include <vector>
#include "engine/core/playercolor.h"

class BotPlayer : public Player
{
    Q_OBJECT
public:
    explicit BotPlayer(QObject *parent, BoardWindow*  board, AI::Bot *bot);
    void connectToBot();
    void disconnectFromBot();
signals:
    void forwardRequestMove(PlayerColor color, const std::vector<Turn>& turns);

public slots:
    //connected to the match
    void chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll) override;
    void setState(const BoardState& state) override;
    void setDice(const Roll& roll) override;

    //connected to bot
    void acceptMove(Turn turn) override;
    void diceRolled(Roll roll) override;

private:
    AI::Bot *m_bot;
    BoardWindow *m_board;
};
