#include "botplayer.h"

BotPlayer::BotPlayer(QObject *parent)
    : Player{parent}
{}

void BotPlayer::askedToDouble(BoardState* state) {
    // poziva bota za dupliranje
    // vraca odgovor preko m_client.sendDoubleToServer() ili m_client.sendNoDoubleToServer()
}
void BotPlayer::receivedState(BoardState* state) {
    // salje state Game-u, traži legal poteze
}

void BotPlayer::chooseMove(std::vector<Turn> &legalMoves) {
    // poziva bota za odabir poteza
    // vraca odgovor preko m_client.sendStateToServer(state)
}
