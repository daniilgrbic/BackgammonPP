#pragma once

#include <QObject>
#include "localplayer.h"
#include "engine/backgammon.h"


class LocalPlayer;

class Match : public QObject
{
    Q_OBJECT
public:
    Match(QObject *parent, LocalPlayer *white, LocalPlayer *black);

    void startGame();

signals:
    void requestMove(std::vector<Turn> *legalMoves, Roll *roll); //override;
    void setState(const BoardState &state); //override;
    void setDice(const Roll &roll);

public slots:
    void getTurn(Turn turn);
    void confirmRoll();
private:
    LocalPlayer *m_white;
    LocalPlayer *m_black;

    LocalPlayer *m_onTurn;
    LocalPlayer *m_waiting;

    //as the idea is to play multiple games, this class should own the game and free it when it is supposed to play another one
    //maybe an enum parameter to the constructor could resolve what gamemode, or smth like that
    Game *game;
    std::vector<Turn> currentLegalTurns;
    Roll currentRoll;

    void connectSlots(LocalPlayer *onMove, LocalPlayer *waiting);
    void startMove();

};

