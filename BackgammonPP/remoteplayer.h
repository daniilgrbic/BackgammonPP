#pragma once

#include <QObject>
#include "player.h"
#include "network/client.h"

class RemotePlayer : public Player
{
    Q_OBJECT
public:
    explicit RemotePlayer(QObject *parent = nullptr, QString ip = "", QString username = "");
    ~RemotePlayer();
    Client *getClient();

signals:
    void terminateGame();

public slots:
    //connected to the match
    void chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll) override;
    void setState(const BoardState& state) override;
    void setDice(const Roll& roll) override;

    //connected to the client
    void acceptMove(Turn turn) override;
    void diceRolled(Roll roll) override;

private:
    Client *m_client;
};


