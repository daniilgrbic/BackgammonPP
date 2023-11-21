#include "backgammon.h"

Backgammon::Backgammon() : Game()
{
    for(auto color : {PlayerColor::WHITE, PlayerColor::BLACK}) {
        m_board.point(Point::idByPlayer(color, 5)).add(color, 5);
        m_board.point(Point::idByPlayer(color, 7)).add(color, 3);
        m_board.point(Point::idByPlayer(color, 13)).add(color, 5);
        m_board.point(Point::idByPlayer(color, 24)).add(color, 2);
    };
}

std::vector<Turn> Backgammon::generateLegalTurns()
{
    return std::vector<Turn>{};
}

GameResult Backgammon::checkFinished()
{
    return m_gameResult;
}
