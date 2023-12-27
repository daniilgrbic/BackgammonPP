#pragma once

#include <QObject>
#include "localplayer.h"
#include "engine/backgammon.h"

class Match : public QObject
{
    Q_OBJECT
public:
    Match(QObject *parent, Player *white, Player *black, int length=1);

    void startGame();

signals:
    void requestMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll); //override;
    void setState(const BoardState &state); //override;
    void setDice(const Roll &roll);
    void sendTurn(const Turn &turn);

public slots:
    void getTurn(Turn turn);
    void confirmRoll(Roll roll);
private:
    Player *m_white;
    Player *m_black;

    Player *m_onTurn;
    Player *m_waiting;

    int m_length;

    int m_whiteScore;
    int m_blackScore;

    //as the idea is to play multiple games, this class should own the game and free it when it is supposed to play another one
    //maybe an enum parameter to the constructor could resolve what gamemode, or smth like that
    Game *game;
    std::vector<Turn> currentLegalTurns;
    Roll currentRoll;

    void endGame();
    void connectSlots(Player *onMove, Player *waiting);
    void startMove(Turn *turn = nullptr);

};

