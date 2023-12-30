#include "longnardy.h"

#include <algorithm>
#include <iterator>

LongNardy::LongNardy() : Game()
{
    m_board.point(24).add(PlayerColor::WHITE, CHECKERS_COUNT);
    m_board.point(Point::centralMirrorId(24)).add(PlayerColor::BLACK, CHECKERS_COUNT);

    // intial play order is determined just like in backgammon,
    // but then the first player throws both dice again (instead of using the intial roll)
    m_currentRoll = Roll(
        Roll::getInitialRoll(m_firstDie, m_secondDie).onRoll(),
        m_firstDie,
        m_secondDie
    );
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

    auto dice = m_currentRoll.dice();
    std::sort(dice.rbegin(), dice.rend());

    std::vector<RollState> level {
        {
            {},
            onRoll == PlayerColor::WHITE ? m_board : BoardState::centralMirror(m_board),
            dice
        }
    };
    std::vector<RollState> nextLevel = level;

    bool doubles = std::count(dice.begin(), dice.end(), dice.front()) == dice.size();

    // first move is doubled when player rolls 33, 44, or 66 and can't perform full move due to opponents head
    if(m_history.size() <= 1 and doubles and dice.front() == 3) {
        std::vector<Move>moves = {{onRoll, 24, 21}, {onRoll, 21, 18}, {onRoll, 18, 15}, {onRoll, 24, 21}};
        return {Turn{onRoll, dice, moves, level.front().getNextRollState(moves, {}).board()}};
    }
    if(m_history.size() <= 1 and doubles and dice.front() == 4 and level.front().board().point(16).count() == 0) {
        std::vector<Move>moves = {{onRoll, 24, 20}, {onRoll, 20, 16}, {onRoll, 24, 20}, {onRoll, 20, 16}};
        return {Turn{onRoll, dice, moves, level.front().getNextRollState(moves, {}).board()}};
    }
    if(m_history.size() <= 1 and doubles and dice.front() == 6) {
        std::vector<Move>moves = {{onRoll, 24, 18}, {onRoll, 24, 18}};
        return {Turn{onRoll, dice, moves, level.front().getNextRollState(moves, {}).board()}};
    }

    do {
        level = std::move(nextLevel);
        nextLevel = {};

        for (RollState& rollState : level) {
            const BoardState& board = rollState.board();
            const std::vector<int>& diceRolls = rollState.dice();

            bool movedFromHead = false;
            for(const Move& move : rollState.moves()) {
                if(std::holds_alternative<int>(move.m_from) and std::get<int>(move.m_from) == NUMBER_OF_POINTS)
                    movedFromHead = true;
            }

            for (size_t i = 0; i < diceRolls.size(); i++) {
                int dieRoll = diceRolls[i];
                if(i > 0 and dieRoll == diceRolls[i-1]) continue;

                for (int pos = NUMBER_OF_POINTS; pos >= 1; pos--) {

                    // player is only allowed to take more than 1 checker from head on first turn
                    // when rolling double 4s or double 6s
                    if(pos == NUMBER_OF_POINTS and movedFromHead)
                        continue;

                    // skip pos if held by opponent
                    if(board.point(pos).count() and board.point(pos).owner().value() != onRoll)
                        continue;

                    // skip pos if unoccupied
                    if(board.point(pos).count() == 0)
                        continue;

                    int nextPos = pos - dieRoll;

                    if (nextPos <= 0 and not isBearingOff(board, onRoll))
                        break;

                    if (nextPos <= 0) {
                        int lastChecker = 0;
                        for(int p = pos; p <= 6; p++)
                            if(board.point(p).count() and board.point(p).owner().value() == onRoll)
                                lastChecker = p;
                        if (nextPos==0 or lastChecker == pos) {
                            auto nextMove = Move(onRoll, pos, SpecialPosition::OFF);
                            nextLevel.push_back(rollState.getNextRollState(nextMove, i));
                        }

                        break;
                    }

                    if (not isBlockedBy(board.point(nextPos), opponent)) {
                        auto nextMove = Move(onRoll, pos, nextPos);
                        nextLevel.push_back(rollState.getNextRollState(nextMove, i));
                    }
                }
            }
        }
    } while (not nextLevel.empty());

    std::vector<Turn> legalTurns;

    for(const auto& roll : level) {

        // check if blocking opponent (a prime in front of all opponent checkers)
        int consecutive = 0;
        bool blockade = false;
        for(int p = 1; p <= NUMBER_OF_POINTS; p++) {
            auto pointOwner = BoardState::centralMirror(roll.board()).point(p).owner();
            if(not pointOwner.has_value()) {
                consecutive = 0;
            }
            else if(pointOwner == opponent) {
                break;
            }
            else if(pointOwner == onRoll){
                consecutive++;
                if(consecutive == 6) {
                    blockade = true;
                    break;
                }
            }
        }

        if(blockade) continue;

        std::vector<Move> moves;
        if (onRoll == PlayerColor::BLACK) {
            std::transform(
                roll.moves().cbegin(), roll.moves().cend(),
                std::back_inserter(moves),
                [](const auto& move) { return Move::centralMirror(move); }
                );
        } else {
            moves = std::move(roll.moves());
        }

        auto turn = Turn {
            onRoll,
            m_currentRoll.dice(),
            moves,
            onRoll == PlayerColor::WHITE ? roll.board() : BoardState::centralMirror(roll.board())
        };

        legalTurns.push_back(turn);
    }

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

std::optional<GameResult> LongNardy::getResult()
{
    if(m_result)
        return m_result;
    if (isFinished(PlayerColor::WHITE)) {
        m_result = GameResult(PlayerColor::WHITE, 1);
    } else if (isFinished(PlayerColor::BLACK)) {
        m_result = GameResult(PlayerColor::BLACK, 1);
    }
    return m_result;
}
