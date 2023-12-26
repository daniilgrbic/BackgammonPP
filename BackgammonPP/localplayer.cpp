#include "localplayer.h"

LocalPlayer::LocalPlayer(QObject *parent, BoardWindow *board)
    : QObject(parent), m_board(board)
{
    connect(this, &LocalPlayer::forwardSetState, board, &BoardWindow::setBoardState);
    connect(this, &LocalPlayer::forwardMoveRequest, board, &BoardWindow::requestTurn);
    connect(board, &BoardWindow::forwardTurnFinish, this, &LocalPlayer::acceptMove);
    connect(this, &LocalPlayer::forwardSetDice, board, &BoardWindow::showRoll);

}

void LocalPlayer::chooseMove(std::vector<Turn> *legalMoves, Roll *roll){
    emit forwardMoveRequest(legalMoves, roll);
}

void LocalPlayer::acceptMove(Turn turn){
    emit returnMove(std::move(turn));
}

void LocalPlayer::setState(const BoardState &state){
    emit forwardSetState(state);
}

void LocalPlayer::setDice(const Roll& roll){
    emit forwardSetDice(roll);
}

void LocalPlayer::diceRolled(){
    emit confirmRoll();
}
