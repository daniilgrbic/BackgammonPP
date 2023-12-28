#include "remoteplayer.h"

RemotePlayer::RemotePlayer(QObject *parent, QString ip, QString username)
    : Player(parent)
{
    // uncomment after fix
    m_client = new Client(ip, username); // uncomment after fix
    connect(this, &RemotePlayer::diceRolled, m_client, &Client::diceRolled);
    connect(this, &RemotePlayer::acceptMove, m_client, &Client::acceptMove);
}

void RemotePlayer::chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll){
    //m_client.sendTurnToServer(turn);
}

void RemotePlayer::acceptMove(Turn turn){
    emit returnMove(std::move(turn));
}

void RemotePlayer::setState(const BoardState &state){
    return;
}

void RemotePlayer::setDice(const Roll& roll){
    //m_client.sendRollToServer(roll);
}

void RemotePlayer::diceRolled(Roll roll){
    emit confirmRoll(roll);
}
