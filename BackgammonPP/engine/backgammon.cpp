#include "backgammon.h"

#include <algorithm>
#include <iterator>

Backgammon::Backgammon() : Game()
{
    for(auto player : {PlayerColor::WHITE, PlayerColor::BLACK}) {
        m_board.point(Point::idByPlayer(player, 5)).add(player, 5);
        m_board.point(Point::idByPlayer(player, 7)).add(player, 3);
        m_board.point(Point::idByPlayer(player, 13)).add(player, 5);
        m_board.point(Point::idByPlayer(player, 24)).add(player, 2);
    };
}

GameResult Backgammon::checkFinished()
{
    return m_gameResult;
}

bool Backgammon::isBlot(int point, PlayerColor player) const {
    const Point& p = m_board.point(Point::idByPlayer(player, point));
    if(p.count() == 0 || p.count() > 1)
        return false;
    return p.owner().value() != player;
}

bool Backgammon::isBlocked(int point, PlayerColor player) const {
    const Point& p = m_board.point(Point::idByPlayer(player, point));
    if(p.count() <= 1)
        return false;
    return p.owner().value() != player;
}

bool Backgammon::isBearingOff(PlayerColor player) const {
    if(m_board.bar(player))
        return false;
    for(int pointId = 24; pointId >= 7; pointId++) {
        auto owner = m_board.point(Point::idByPlayer(player, pointId)).owner();
        if(owner.has_value() && owner.value() == player)
            return false;
    }
    return true;
}

// TODO:
// - makes it work for both directions (currently only works for 24 -> 1)
// - refactor function
std::vector<Turn> Backgammon::generateLegalTurns() {

    class RollState {
    public:
        RollState(const std::vector<Move>& moves, const BoardState& board, const std::vector<int>& dice)
            : m_moves { moves}, m_board { board }, m_dice { dice }
        {}

        const BoardState& board() const { return m_board; }
        const std::vector<int>& dice() const { return m_dice; }
        const std::vector<Move>& moves() const { return m_moves; }

        RollState getNextRollState(const Move move, int dieUsed) const {
            auto nextMoves = m_moves;
            nextMoves.push_back(move);
            auto nextState = m_board.getNextState(move);
            auto nextDice = m_dice;
            nextDice.erase(std::next(nextDice.begin(), dieUsed));
            return { nextMoves, nextState, nextDice };
        }

        RollState getNextRollState(const std::vector<Move> moves, int dieUsed) const {
            auto nextMoves = m_moves;
            nextMoves.insert(nextMoves.end(), moves.begin(), moves.end());
            auto nextState = m_board.getNextState(moves);
            auto nextDice = m_dice;
            nextDice.erase(std::next(nextDice.begin(), dieUsed));
            return { nextMoves, nextState, nextDice };
        }

    private:
        std::vector<Move> m_moves;
        BoardState m_board;
        std::vector<int> m_dice;
    };

    auto onRoll = m_currentRoll.onRoll();
    auto opponent = onRoll == PlayerColor::BLACK ? PlayerColor::WHITE : PlayerColor::BLACK;
    std::vector<int> dice;
    auto board = m_board;

    std::vector<RollState> level {{{}, board, dice}};
    std::vector<RollState> nextLevel = level;

    do {
        level = std::move(nextLevel);
        nextLevel = {};

        for (auto& roll: level) {
            const auto& board = roll.board();
            const auto& dice = roll.dice();

            for (size_t i = 0; i < dice.size(); ++i) {
                auto die = dice[i];
                if (board.bar(onRoll)) {
                    auto nextPos = Point::idByPlayer(onRoll, NUMBER_OF_POINTS - die);
                    if (!isBlocked(nextPos, opponent) && !isBlot(nextPos, opponent)) {
                        auto nextMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                        nextLevel.push_back(roll.getNextRollState(nextMove, i));
                    } else if (isBlot(nextPos, opponent)) {
                        auto opponentMove = Move(opponent, nextPos, SpecialPosition::BAR);
                        auto onRollMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                        nextLevel.push_back(roll.getNextRollState({ opponentMove, onRollMove }, i));
                    }
                } else for (int pos = NUMBER_OF_POINTS - 1; pos >= 0; --pos) {
                    if (board.point(pos).owner() && board.point(pos).owner().value() == onRoll) {
                        auto nextPos = pos - die;
                        if (nextPos < 0) {
                            if (isBearingOff(onRoll)) {
                                auto nextMove = Move(onRoll, pos, SpecialPosition::OFF);
                                nextLevel.push_back(roll.getNextRollState(nextMove, i));
                            }
                            break;
                        } else if (!isBlocked(nextPos, opponent) && !isBlot(nextPos, opponent)) {
                            auto nextMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                            nextLevel.push_back(roll.getNextRollState(nextMove, i));
                        } else if (isBlot(nextPos, opponent)) {
                            auto opponentMove = Move(opponent, nextPos, SpecialPosition::BAR);
                            auto onRollMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                            nextLevel.push_back(roll.getNextRollState({ opponentMove, onRollMove }, i));
                        }
                    }
                }
            }
        }
    } while (!nextLevel.empty());

    std::vector<Turn> legalTurns;
    // TODO: index
    std::transform(
        level.cbegin(), level.cend(),
        std::back_inserter(legalTurns),
        [onRoll](const RollState& roll) { return Turn { 0, onRoll, roll.moves() }; }
    );
    return legalTurns;
}
