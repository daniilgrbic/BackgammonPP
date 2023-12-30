#include "match.h"
#include "engine/backgammon.h"
#include "engine/longnardy.h"
#include "remoteplayer.h"

#include <algorithm>
#include <vector>
#include <QMessageBox>

Match::Match(QObject *parent, Player *white, Player *black, BoardWindow *gboard, int length, GameType gameType, bool host)
    : QObject(parent),
      m_white(white),
      m_black(black),
      m_length(length),
      m_gameType(gameType),
      m_whiteScore(0),
      m_blackScore(0),
      m_host(host),
      m_gBoard(gboard),
      m_spectator(false)
{
}

Match::~Match() {
    if (game)
        delete game;
}

void Match::startGame() {
    switch (m_gameType) {
    case GameType::ClassicGameType:
        game = new Backgammon();
        break;
    case GameType::LongNardyGameType:
        game = new LongNardy();
        break;
    default:
        game = new Backgammon();
        break;
    }

    m_gBoard->setExitPoints(m_gameType);

    connect(this, &Match::setState, m_white, &Player::setState);
    emit setState(game->board());
    disconnect(this, &Match::setState, m_white, &Player::setState);

    connect(this, &Match::setState, m_black, &Player::setState);
    emit setState(game->board());
    disconnect(this, &Match::setState, m_black, &Player::setState);

    m_gBoard->m_historyModel->addString("--- Start of match ---");
    m_gBoard->scrollLogToBottom();

    PlayerColor first = game->currentRoll().onRoll();
    if(first == PlayerColor::WHITE){
        m_onTurn = m_white;
        m_waiting = m_black;
    }else{
        m_onTurn = m_black;
        m_waiting = m_white;
    }
    connectSlots(m_onTurn, m_waiting);
    if (m_host) {
        startMove();
    }
}

void Match::startGameRequest() {
    startGame();
}

void Match::connectedAsPlayer(int length, GameType gameType) {
    m_length = length;
    m_gameType = gameType;
    startGame();
}

void Match::connectedAsSpectator(int length, GameType gameType) {
    m_length = length;
    m_gameType = gameType;
    m_spectator = true;
    startGame();
}


void Match::startMove(Turn *turn) {
    currentLegalTurns = game->generateLegalTurns();
    currentRoll = Roll(game->currentRoll());
    emit requestMove(turn, &currentLegalTurns, &currentRoll);
}

void Match::confirmRoll(Roll roll){
    emit setDice(roll);
}

void Match::getTurn(Turn turn){
    //TODO: Match has to be changed. It cannot be responsible for the game
    if (m_spectator) {
        emit setState(turn.m_finalBoard);
        return;
    }
    game->playTurn(turn);
    emit setState(game->board());
    if(game->isFinished(PlayerColor::WHITE) || game->isFinished(PlayerColor::BLACK)) {
        if (dynamic_cast<RemotePlayer*>(m_waiting) != nullptr) {
            std::swap(m_onTurn, m_waiting);
            connectSlots(m_onTurn, m_waiting);
            emit requestMove(&turn, nullptr, nullptr);
        }
        endGame();
    } else {
        std::swap(m_onTurn, m_waiting);
        connectSlots(m_onTurn, m_waiting);
        startMove(&turn);
    }
    m_gBoard->scrollLogToBottom();
}

void Match::endGame() {
    auto result = game->getResult().value();
    auto& winnerPoints = result.winner == PlayerColor::WHITE ? m_whiteScore : m_blackScore;
    winnerPoints += result.points;

    // Debug log
    qDebug() << "WHITE: " << m_whiteScore << "\t" << "BLACK: " << m_blackScore;

    // Set scores
    m_gBoard->setScore(m_whiteScore, m_blackScore);
    QString winner = result.winner == PlayerColor::WHITE ? "WHITE" : "BLACK";
    QMessageBox::information(nullptr, "Game Result",
                             QString("%1 won!\nScore: white %2 - black %3").arg(winner).arg(m_whiteScore).arg(m_blackScore));

    m_gBoard->m_historyModel->addString("---- End of match ----");
    m_gBoard->scrollLogToBottom();

    delete game;
    game = nullptr;
    if (winnerPoints < m_length) {
        startGame();
    }
}

void Match::connectSlots(Player *onMove, Player *waiting){
    disconnect(this, &Match::requestMove, waiting, &Player::chooseMove);
    disconnect(this, &Match::setDice, onMove, &Player::setDice);
    disconnect(this, &Match::setState, onMove, &Player::setState);

    disconnect(waiting, &Player::returnMove, this, &Match::getTurn);
    disconnect(waiting, &Player::confirmRoll, this, &Match::confirmRoll);

    connect(this, &Match::requestMove, onMove, &Player::chooseMove);
    connect(this, &Match::setDice, waiting, &Player::setDice);
    connect(this, &Match::setState, waiting, &Player::setState);

    connect(onMove, &Player::returnMove, this, &Match::getTurn);
    connect(onMove, &Player::confirmRoll, this, &Match::confirmRoll);

}
