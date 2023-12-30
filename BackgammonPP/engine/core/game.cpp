#include "game.h"

Game::Game()
    : m_board(BoardState{})
    , m_history(std::vector<Turn>{})
{ }

Game::~Game()
{}

void Game::playTurn(Turn turn)
{
    for(const auto& move : turn.m_moves)
    {
        m_board.move(move);
    }

    m_history.push_back(turn);
    m_currentRoll = m_currentRoll.getNextRoll(m_firstDie, m_secondDie);
}

const Roll& Game::currentRoll() const {
    return m_currentRoll;
}

const BoardState& Game::board() const {
    return m_board;
}

bool Game::isBlot(const Point& point, PlayerColor player) const {
    return point.count() == 1 && point.owner().value() == player;
}

bool Game::isBlockedBy(const Point& point, PlayerColor player) const {
    return point.count() >= 2 && point.owner().value() == player;
}

bool Game::isBearingOff(const BoardState& board, PlayerColor player) const {
    if(board.bar(player))
        return false;
    for(int pointId = 24; pointId >= 7; --pointId) {
        auto owner = board.point(pointId).owner();
        if(owner && owner.value() == player)
            return false;
    }
    return true;
}
