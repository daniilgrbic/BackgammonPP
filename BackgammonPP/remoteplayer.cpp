#include "remoteplayer.h"

RemotePlayer::RemotePlayer(QObject *parent, QString ip, QString username)
    : Player(parent)
{
    m_client = new Client(ip, username); // uncomment after fix
    connect(m_client, &Client::diceRolled, this, &RemotePlayer::diceRolled);
    connect(m_client, &Client::sendMove, this, &RemotePlayer::acceptMove);
}

void RemotePlayer::chooseMove(Turn *turn, std::vector<Turn> *legalMoves, Roll *roll){
    m_client->sendTurnToServer(turn);
}

void RemotePlayer::acceptMove(Turn turn){
    emit returnMove(std::move(turn));
}

Client* RemotePlayer::getClient() {
    return m_client;
}


void RemotePlayer::setState(const BoardState &state){
    return;
}

void RemotePlayer::setDice(const Roll& roll){
    m_client->sendRollToServer(roll);
}

void RemotePlayer::diceRolled(Roll roll){
    emit confirmRoll(roll);
}
