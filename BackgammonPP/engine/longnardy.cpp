#include "longnardy.h"

#include <algorithm>
#include <iterator>

LongNardy::LongNardy() : Game()
{
    for(auto player : {PlayerColor::WHITE, PlayerColor::BLACK}) {
        m_board.point(Point::idByPlayer(player, 24)).add(player, 15);
    };

    m_currentRoll = Roll::getInitialRoll(m_firstDie, m_secondDie);
}

std::vector<Turn> LongNardy::generateLegalTurns() {

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

    PlayerColor onRoll = m_currentRoll.onRoll();
    PlayerColor opponent = (onRoll == PlayerColor::WHITE) ? PlayerColor::BLACK : PlayerColor::WHITE;

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

        for (RollState& rollState : level) {
            const BoardState& board = rollState.board();
            const std::vector<int>& diceRolls = rollState.dice();

            for (size_t i = 0; i < diceRolls.size(); i++) {
                int dieRoll = diceRolls[i];
                for (int pos = NUMBER_OF_POINTS; pos >= 1; pos--) {
                    if (board.point(pos).owner() && board.point(pos).owner().value() == onRoll) {
                        int nextPos = pos - dieRoll;
                        if (nextPos <= 0) {
                            if (isBearingOff(board, onRoll)) {
                                auto nextMove = Move(onRoll, pos, SpecialPosition::OFF);
                                nextLevel.push_back(rollState.getNextRollState(nextMove, i));
                            }
                            break;
                        } else if (not isBlockedBy(board.point(nextPos), opponent)) {
                            auto nextMove = Move(onRoll, pos, nextPos);
                            nextLevel.push_back(rollState.getNextRollState(nextMove, i));
                        }
                    }
                }
            }
        }
    } while (not nextLevel.empty());

    std::vector<Turn> legalTurns;
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

bool LongNardy::isFinished(PlayerColor player) const
{
    return m_board.off(player) == CHECKERS_COUNT;
}

bool LongNardy::isBlockedBy(const Point &point, PlayerColor player) const
{
    return point.count() >= 1 && point.owner().value() == player;
}
