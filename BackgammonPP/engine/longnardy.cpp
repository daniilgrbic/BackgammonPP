#include "longnardy.h"

#include <algorithm>
#include <iterator>

LongNardy::LongNardy() : Game()
{
    m_board.point(pointIdByPlayer(PlayerColor::WHITE, 24)).add(PlayerColor::WHITE, 15);
    m_board.point(pointIdByPlayer(PlayerColor::BLACK, 24)).add(PlayerColor::BLACK, 15);

    // intial play order is determined just like in backgammon,
    // but then the first player throws both dice again (instead of using the intial roll)
    m_currentRoll = Roll(
        Roll::getInitialRoll(m_firstDie, m_secondDie).onRoll(),
        m_firstDie,
        m_secondDie
        );
}

int LongNardy::pointIdByPlayer(PlayerColor player, int point)
{
    if(player == PlayerColor::WHITE)
        return point;
    else
        return (point >= 1 and point <= 12) ? point + 12 : point - 12;
}

BoardState LongNardy::mirrorBoard(BoardState boardState)
{
    for(int i = 1; i <= 12; i++) {
        std::swap(boardState.point(i), boardState.point(25 - i));
    }
    return boardState;
}

Move LongNardy::mirrorMove(Move move)
{
    Move newMove = move;
    if(std::holds_alternative<int>(move.m_from))
        newMove.m_from = pointIdByPlayer(move.m_player, std::get<int>(move.m_from));
    if(std::holds_alternative<int>(move.m_to))
        newMove.m_to = pointIdByPlayer(move.m_player, std::get<int>(move.m_to));
    return newMove;
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
            onRoll == PlayerColor::WHITE ? m_board : mirrorBoard(m_board),
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

            int movesFromHead = 0;
            for(const Move& move : rollState.moves()) {
                if(std::holds_alternative<int>(move.m_from) and std::get<int>(move.m_from) == NUMBER_OF_POINTS)
                    movesFromHead++;
            }

            for (size_t i = 0; i < diceRolls.size(); i++) {
                int dieRoll = diceRolls[i];
                for (int pos = NUMBER_OF_POINTS; pos >= 1; pos--) {

                    // player is only allowed to take more than 1 checker from head on first turn
                    if(pos == NUMBER_OF_POINTS) {
                        if(movesFromHead == 2) continue;
                        if(movesFromHead == 1 and m_history.empty()) continue;
                    }

                    // skip pos if held by opponent
                    if(board.point(pos).owner() and board.point(pos).owner().value() != onRoll)
                        continue;

                    // skip pos if unoccupied
                    if(board.point(pos).count() == 0)
                        continue;

                    int nextPos = pos - dieRoll;

                    if (nextPos <= 0) {
                        if (isBearingOff(board, onRoll)) {
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
    for(const auto& rollState : level) {
        // TODO: make this pretty and optimized
        std::vector<Move> moves;
        if (onRoll == PlayerColor::BLACK) {
            std::transform(
                rollState.moves().cbegin(), rollState.moves().cend(),
                std::back_inserter(moves),
                [](const auto& move) { return mirrorMove(move); }
            );
        } else {
            moves = std::move(rollState.moves());
        }
        auto newTurn = Turn { 0, onRoll, moves, onRoll == PlayerColor::WHITE ? rollState.board() : mirrorBoard(rollState.board()) };
        bool unique = true;
        for(const auto& turn : legalTurns) {
            if(turn.m_finalBoard == newTurn.m_finalBoard) {
                unique = false;
                break;
            }
        }
        if(unique) {
            legalTurns.push_back(newTurn);
        }
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
