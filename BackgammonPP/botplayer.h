#ifndef BOTPLAYER_H
#define BOTPLAYER_H

#include <QObject>
#include "player.h"

class BotPlayer : public Player
{
    Q_OBJECT
public:
    explicit BotPlayer(QObject *parent = nullptr);

public slots:
    void askedToDouble(BoardState* state) override;
    void receivedState(BoardState* state) override;
    void chooseMove(std::vector<Turn> &legalMoves) override;
};

#endif // BOTPLAYER_H
