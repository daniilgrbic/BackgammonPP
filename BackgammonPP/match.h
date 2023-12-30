#pragma once

#include "player.h"
#include "consts.h"
#include "engine/core/game.h"
#include "boardwindow.h"

#include <QObject>

class Match : public QObject
{
    Q_OBJECT
public:
    Match(QObject *parent, Player *white, Player *black, BoardWindow *gBoard, int length=1, GameType gameType=GameType::ClassicGameType, bool host = true);

    ~Match();

    void startGame();

signals:
    void requestMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll); //override;
    void setState(const BoardState &state); //override;
    void setDice(const Roll &roll);
    void sendTurn(const Turn &turn);

public slots:
    void getTurn(Turn turn);
    void confirmRoll(Roll roll);
    void startGameRequest();
    void connectedAsPlayer(int length, GameType gameType);
    void connectedAsSpectator(int length, GameType gameType);

private:
    Player *m_white;
    Player *m_black;

    Player *m_onTurn;
    Player *m_waiting;

    BoardWindow *m_gBoard;

    int m_length;
    bool m_host;
    bool m_spectator;

    GameType m_gameType;

    int m_whiteScore;
    int m_blackScore;

    //as the idea is to play multiple games, this class should own the game and free it when it is supposed to play another one
    //maybe an enum parameter to the constructor could resolve what gamemode, or smth like that
    Game *game { nullptr };
    std::vector<Turn> currentLegalTurns;
    Roll currentRoll;

    void endGame();
    void connectSlots(Player *onMove, Player *waiting);
    void startMove(Turn *turn = nullptr);

};

