#include "backgammon.h"

#include <algorithm>
#include <iterator>

Backgammon::Backgammon() : Game()
{
    for(auto player : {PlayerColor::WHITE, PlayerColor::BLACK}) {
        m_board.point(Point::idByPlayer(player, 6)).add(player, 5);
        m_board.point(Point::idByPlayer(player, 8)).add(player, 3);
        m_board.point(Point::idByPlayer(player, 13)).add(player, 5);
        m_board.point(Point::idByPlayer(player, 24)).add(player, 2);
    };

    m_currentRoll = Roll::getInitialRoll(m_firstDie, m_secondDie);
}

// WIP
// TODO:
// - exhaustive testing: currently tested only for regular moves (from point to point, without blots)
// - refactor function
std::vector<Turn> Backgammon::generateLegalTurns() {

    class RollState {
    public:
        RollState(const std::vector<Move>& moves, const BoardState& board, const std::vector<int>& dice)
            : m_moves { moves }, m_board { board }, m_dice { dice }
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
    auto opponent = onRoll == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE;

    std::vector<RollState> level {
        {
            {},
            onRoll == PlayerColor::WHITE ? m_board : m_board.mirror(),
            m_currentRoll.dice()
        }
    };
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
                    auto nextPos = NUMBER_OF_POINTS - die;
                    if (!isBlockedBy(board.point(nextPos), opponent) && !isBlot(board.point(nextPos), opponent)) {
                        auto nextMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                        nextLevel.push_back(roll.getNextRollState(nextMove, i));
                    } else if (isBlot(board.point(nextPos), opponent)) {
                        auto opponentMove = Move(opponent, nextPos, SpecialPosition::BAR);
                        auto onRollMove = Move(onRoll, SpecialPosition::BAR, nextPos);
                        nextLevel.push_back(roll.getNextRollState({ opponentMove, onRollMove }, i));
                    }
                } else for (int pos = NUMBER_OF_POINTS; pos >= 1; --pos) {
                    if (board.point(pos).owner() && board.point(pos).owner().value() == onRoll) {
                        auto nextPos = pos - die;
                        if (nextPos <= 0) {
                            if (isBearingOff(board, onRoll)) {
                                auto nextMove = Move(onRoll, pos, SpecialPosition::OFF);
                                nextLevel.push_back(roll.getNextRollState(nextMove, i));
                            }
                            break;
                        } else if (!isBlockedBy(board.point(nextPos), opponent) && !isBlot(board.point(nextPos), opponent)) {
                            auto nextMove = Move(onRoll, pos, nextPos);
                            nextLevel.push_back(roll.getNextRollState(nextMove, i));
                        } else if (isBlot(board.point(nextPos), opponent)) {
                            auto opponentMove = Move(opponent, nextPos, SpecialPosition::BAR);
                            auto onRollMove = Move(onRoll, pos, nextPos);
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
        [onRoll](const RollState& roll) {
            std::vector<Move> moves;
            if (onRoll == PlayerColor::BLACK) {
                std::transform(
                    roll.moves().cbegin(), roll.moves().cend(),
                    std::back_inserter(moves),
                    [](const auto& move) { return move.mirror(); }
                );
            } else {
                moves = std::move(roll.moves());
            }

            return Turn { 0, onRoll, moves, onRoll == PlayerColor::WHITE ? roll.board() : roll.board().mirror() }; }
    );
    return legalTurns;
}

bool Backgammon::isFinished(PlayerColor player) const
{
    return m_board.off(player) == CHECKERS_COUNT;
}
