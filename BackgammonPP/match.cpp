#include "match.h"
#include <algorithm>
#include <vector>

Match::Match(QObject *parent, LocalPlayer *white, LocalPlayer *black)
    : QObject(parent),
      m_white(white),
      m_black(black)
{
    startGame();
}

void Match::startGame(){
    game = new Backgammon();
    connect(this, &Match::setState, m_white, &LocalPlayer::setState);
    emit setState(game->board());
    connect(this, &Match::setState, m_black, &LocalPlayer::setState);
    emit setState(game->board());

    PlayerColor first = game->currentRoll().onRoll();
    if(first == PlayerColor::WHITE){
        m_onTurn = m_white;
        m_waiting = m_black;
    }else{
        m_onTurn = m_black;
        m_waiting = m_white;
    }
    connectSlots(m_onTurn, m_waiting);
    startMove();
}


void Match::startMove(){
    currentLegalTurns = game->generateLegalTurns();
    currentRoll = Roll(game->currentRoll());
    emit requestMove(&currentLegalTurns, &currentRoll);
}

void Match::confirmRoll(){
    emit setDice(currentRoll);
}

void Match::getTurn(Turn turn){
    game->playTurn(turn);
    emit setState(game->board());
    if(game->isFinished(PlayerColor::WHITE) || game->isFinished(PlayerColor::BLACK)){
        //HAS TO BE IMPLEMENTED
        exit(1);
    }
    std::swap(m_onTurn, m_waiting);
    startMove();

}

void Match::connectSlots(LocalPlayer *onMove, LocalPlayer *waiting){
    connect(this, &Match::requestMove, onMove, &LocalPlayer::chooseMove);
    connect(this, &Match::setDice, waiting, &LocalPlayer::setDice);
    connect(this, &Match::setState, waiting, &LocalPlayer::setState);

    connect(onMove, &LocalPlayer::returnMove, this, &Match::getTurn);
    connect(onMove, &LocalPlayer::confirmRoll, this, &Match::confirmRoll);

}
