#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <vector>
#include "network/Client.h"
#include "engine/core/boardstate.h"
#include "engine/core/playercolor.h"
#include "engine/core/turn.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    bool connectToServer(QString ip, QString username);

signals:
    void gameEnded(int gameValue);
    void matchEnded();
    void requestLastBoardState();

public slots:
    void toDouble();
    void toNotDouble();
    void oppDoubled();
    void askToDouble();
    void endGame();
    virtual void askedToDouble(BoardState* state) = 0;
    virtual void receivedState(BoardState* state) = 0;
    virtual void chooseMove(std::vector<Turn> &legalMoves) = 0;

private:
    Client* m_client;
    PlayerColor m_playerColor;
};

#endif // PLAYER_H
