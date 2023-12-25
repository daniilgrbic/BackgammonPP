#include "localplayer.h"

LocalPlayer::LocalPlayer(QObject *parent)
    : PlayablePlayer{parent}
{}

void LocalPlayer::askedToDouble(BoardState* state) {

}

void LocalPlayer::receivedState(BoardState* state) {

}

void LocalPlayer::chooseMove(std::vector<Turn> &legalMoves) {

}

void LocalPlayer::acceptDice(int diceValue) {

}
