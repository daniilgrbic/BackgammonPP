#include "localplayer.h"

LocalPlayer::LocalPlayer(QObject *parent, BoardWindow *board)
    : Player(parent), m_board(board)
{
    connect(this, &LocalPlayer::forwardSetState, board, &BoardWindow::setBoardState);
    connect(this, &LocalPlayer::forwardMoveRequest, board, &BoardWindow::requestTurn);
    connect(board, &BoardWindow::diceRolled, this, &LocalPlayer::diceRolled);
    connect(this, &LocalPlayer::forwardSetDice, board, &BoardWindow::showRoll);
}

void LocalPlayer::chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll){
    connectToBoard();
    emit forwardMoveRequest(legalMoves, roll);
}

void LocalPlayer::acceptMove(Turn turn){
    disconnectFromBoard();
    emit returnMove(std::move(turn));
}

void LocalPlayer::setState(const BoardState &state){
    emit forwardSetState(state);
}

void LocalPlayer::setDice(const Roll& roll){
    emit forwardSetDice(roll);
}

void LocalPlayer::diceRolled(Roll roll){
    emit confirmRoll(roll);
}

void LocalPlayer::connectToBoard(){
    connect(m_board, &BoardWindow::forwardTurnFinish, this, &LocalPlayer::acceptMove);
}

void LocalPlayer::disconnectFromBoard(){
    disconnect(m_board, &BoardWindow::forwardTurnFinish, this, &LocalPlayer::acceptMove);
}
