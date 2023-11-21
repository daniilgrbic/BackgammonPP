#include "game.h"

Game::Game()
    : m_board(BoardState{})
    , m_history(std::vector<Turn>{})
    , m_gameResult(GameResult::UNRESOLVED)
{ }

GameResult Game::playTurn(Turn turn)
{
    for(const auto move : turn.moves)
    {
        m_board.move(move);
    }
    return checkFinished();
}
