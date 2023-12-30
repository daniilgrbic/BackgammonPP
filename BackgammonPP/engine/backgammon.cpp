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

bool Backgammon::isGammon() const {
    return (isFinished(PlayerColor::WHITE) && !m_board.off(PlayerColor::BLACK)) ||
           (isFinished(PlayerColor::BLACK) && !m_board.off(PlayerColor::WHITE));
}

bool Backgammon::isBackgammon() const {
    constexpr auto QUADRANT_SIZE = NUMBER_OF_POINTS / 4;
    if (isFinished(PlayerColor::WHITE) && !m_board.off(PlayerColor::BLACK)) {
        for (auto i = 1; i <= QUADRANT_SIZE; ++i)
            if (m_board.point(i).owner().value_or(PlayerColor::WHITE) == PlayerColor::BLACK)
                return true;
    }
    if (isFinished(PlayerColor::BLACK) && !m_board.off(PlayerColor::WHITE)) {
            for (auto i = 1; i <= QUADRANT_SIZE; ++i)
                if (m_board.point(NUMBER_OF_POINTS - i + 1).owner().value_or(PlayerColor::BLACK) == PlayerColor::WHITE)
                    return true;
    }
    return false;
}

std::optional<GameResult> Backgammon::getResult() {
    if (m_result)
        return m_result;

    if (isFinished(PlayerColor::WHITE)) {
        auto points = 1;
        if (isGammon()) points = 2;
        if (isBackgammon()) points = 3;
        m_result = GameResult(PlayerColor::WHITE, points);
    } else if (isFinished(PlayerColor::BLACK)) {
        auto points = 1;
        if (isGammon()) points = 2;
        if (isBackgammon()) points = 3;
        m_result = GameResult(PlayerColor::BLACK, points);
    }
    return m_result;
}

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

    private:
        std::vector<Move> m_moves;
        BoardState m_board;
        std::vector<int> m_dice;
    };

    auto onRoll = m_currentRoll.onRoll();
    auto opponent = onRoll == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE;

    auto dice = m_currentRoll.dice();
    std::sort(dice.rbegin(), dice.rend());

    std::vector<RollState> level {
        {
            {},
            onRoll == PlayerColor::WHITE ? m_board : m_board.mirror(),
            dice
        }
    };
    std::vector<RollState> nextLevel = level;

    do {
        level = std::move(nextLevel);
        nextLevel = {};

        for (RollState& roll: level) {
            const BoardState& board = roll.board();
            const std::vector<int>& dice = roll.dice();

            for (size_t i = 0; i < dice.size(); ++i) {
                auto die = dice[i];
                if (board.bar(onRoll)) {
                    auto nextPos = NUMBER_OF_POINTS - die + 1;
                    if (!isBlockedBy(board.point(nextPos), opponent)) {
                        auto nextMove = Move(onRoll, SpecialPosition::BAR, nextPos, isBlot(board.point(nextPos), opponent));
                        nextLevel.push_back(roll.getNextRollState(nextMove, i));
                    }
                } else for (int pos = NUMBER_OF_POINTS; pos >= 1; --pos) {

                    // skip pos if held by opponent
                    if(board.point(pos).count() and board.point(pos).owner().value() != onRoll)
                        continue;

                    // skip pos of unoccupied
                    if(board.point(pos).count() == 0)
                        continue;

                    int nextPos = pos - die;

                    if (nextPos <= 0 and not isBearingOff(board, onRoll))
                        break;

                    if (nextPos <= 0) {
                        int lastChecker = 0;
                        for(int p = pos; p <= 6; p++)
                            if(board.point(p).count() and board.point(p).owner().value() == onRoll)
                                lastChecker = p;
                        if (nextPos==0 or lastChecker == pos) {
                            auto nextMove = Move(onRoll, pos, SpecialPosition::OFF);
                            nextLevel.push_back(roll.getNextRollState(nextMove, i));
                        }
                        break;
                    }

                    if (!isBlockedBy(board.point(nextPos), opponent)) {
                        auto nextMove = Move(onRoll, pos, nextPos, isBlot(board.point(nextPos), opponent));
                        nextLevel.push_back(roll.getNextRollState(nextMove, i));
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
        [onRoll, this](const RollState& roll) {
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

            return Turn { onRoll, m_currentRoll.dice(), moves, onRoll == PlayerColor::WHITE ? roll.board() : roll.board().mirror() };
        }
    );
    return legalTurns;
}

bool Backgammon::isFinished(PlayerColor player) const
{
    return m_board.off(player) == CHECKERS_COUNT;
}
