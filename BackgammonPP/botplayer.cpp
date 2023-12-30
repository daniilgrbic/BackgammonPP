#include "botplayer.h"
#include "engine/bot/bot.hpp"
#include "boardwindow.h"
#include "historylistmodel.h"

BotPlayer::BotPlayer(QObject *parent, BoardWindow *board, AI::Bot *bot)
    : Player{parent}, m_bot{bot}, m_board{board}
{
    // connect(this, &BotPlayer::forwardSetState, board, &BoardWindow::setBoardState);
    //connect(this, &LocalPlayer::forwardMoveRequest, board, &BoardWindow::requestTurn);
    // connect(board, &BoardWindow::forwardTurnFinish, this, &BotPlayer::acceptMove);
    // connect(this, &BotPlayer::forwardSetDice, board, &BoardWindow::showRoll);
    // connect(this, &BotPlayer::returnMove, board->m_historyModel, &HistoryListModel::addTurn);
    bot->setParent(this);
    connect(this, &BotPlayer::forwardRequestMove, m_bot, &AI::Bot::requestBestMove);
    connect(m_bot, &AI::Bot::bestMove, this, &BotPlayer::acceptMove);
}

void BotPlayer::chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll){
    // Turn bestTurn = m_bot->bestTurn(m_playerColor, *legalMoves);
    diceRolled(*roll);
    connectToBot();
    emit forwardRequestMove(roll->onRoll(), *legalMoves);

}
void BotPlayer::setState(const BoardState &state){
    // emit forwardSetState(state);
}

void BotPlayer::setDice(const Roll& roll){
    // emit forwardSetDice(roll);
}

//connected to bot
void BotPlayer::acceptMove(Turn turn) {
    disconnectFromBot();
    emit returnMove(turn);
}
void BotPlayer::diceRolled(Roll roll) {
    emit confirmRoll(roll);
}

void BotPlayer::connectToBot(){
    connect(m_bot, &AI::Bot::bestMove, this, &BotPlayer::acceptMove);
}

void BotPlayer::disconnectFromBot(){
    disconnect(m_bot, &AI::Bot::bestMove, this, &BotPlayer::acceptMove);
}
