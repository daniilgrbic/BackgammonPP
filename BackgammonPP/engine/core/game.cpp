#include "game.h"

Game::Game()
    : m_board(BoardState{})
    , m_history(std::vector<Turn>{})
{ }

void Game::playTurn(Turn turn)
{
    for(const auto move : turn.m_moves)
    {
        m_board.move(move);
    }
}

std::optional<GameResult> Game::getResult() const
{
    return m_result;
}

bool Game::isBlot(int point, PlayerColor player) const {
    const Point& p = m_board.point(Point::idByPlayer(player, point));
    if(p.count() == 0 || p.count() > 1)
        return false;
    return p.owner().value() != player;
}

bool Game::isBlocked(int point, PlayerColor player) const {
    const Point& p = m_board.point(Point::idByPlayer(player, point));
    if(p.count() <= 1)
        return false;
    return p.owner().value() != player;
}

bool Game::isBearingOff(PlayerColor player) const {
    if(m_board.bar(player))
        return false;
    for(int pointId = 24; pointId >= 7; pointId++) {
        auto owner = m_board.point(Point::idByPlayer(player, pointId)).owner();
        if(owner.has_value() && owner.value() == player)
            return false;
    }
    return true;
}
