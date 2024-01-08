#include <catch2/catch_test_macros.hpp>

#include "../engine/backgammon.h"
#include "../engine/core/turntrie.h"

#include <algorithm>

std::vector<Turn> turnTrieTurns(TurnTrie& trie) {
    if (trie.isFinishedTurn())
        return { trie.getTurn() };

    std::vector<Turn> turns;
    for (auto& move : trie.nextMoves()) {
        trie.playMove(move);
        for (const auto& turn : turnTrieTurns(trie))
            turns.push_back(turn);
        trie.undoMove();
    }
    return turns;
}

TEST_CASE("TurnTrie is correctly constructed") {

    SECTION("Turns of TurnTrie are subset of turns argument") {
        // Arrange
        auto game = Backgammon();
        auto turns = game.generateLegalTurns();
        auto board = game.board();

        // Act
        auto trie = TurnTrie(turns, board);
        auto trieTurns = turnTrieTurns(trie);

        // Assert
        for (const auto& turn : trieTurns) {
            auto it = std::find(turns.begin(), turns.end(), turn);
            REQUIRE(it != turns.end());
        }
    }

    SECTION("Turns of TurnTrie are superset of turns argument") {
        // Arrange
        auto game = Backgammon();
        auto turns = game.generateLegalTurns();
        auto board = game.board();

        // Act
        auto trie = TurnTrie(turns, board);
        auto trieTurns = turnTrieTurns(trie);

        // Assert
        for (const auto& turn : turns) {
            auto it = std::find(trieTurns.begin(), trieTurns.end(), turn);
            REQUIRE(it != trieTurns.end());
        }
    }
}

TEST_CASE("TurnTrie is Turn traversable") {

    SECTION("Playing moves of one turn should result in playing that same turn") {
        // Arrange
        auto board = Backgammon().board();
        std::vector<Move> moves { Move(PlayerColor::WHITE, 24, 23), Move(PlayerColor::WHITE, 24, 22)};
        auto finalBoard = board.getNextState(moves);
        auto turn = Turn(PlayerColor::WHITE, { 1, 2 }, moves, board);
        auto trie = TurnTrie({ turn }, board);
        auto legalMoves = true;
        auto prematureFinished = false;

        // Act
        for (const auto& move : moves) {
            if (!trie.hasNextMove(move)) {
                legalMoves = false;
                break;
            }
            if (trie.isFinishedTurn()) {
                prematureFinished = true;
                break;
            }
            trie.playMove(move);
        }

        // Assert
        REQUIRE(legalMoves);
        REQUIRE_FALSE(prematureFinished);
        REQUIRE(trie.isFinishedTurn());
        REQUIRE(trie.getTurn() == turn);
        REQUIRE(trie.board() == finalBoard);
    }
}

TEST_CASE("TurnTrie undo operation") {

    SECTION("Playing move and unoding it results in doing nothing") {
        // Arrange
        auto board = Backgammon().board();
        std::vector<Move> moves { Move(PlayerColor::WHITE, 24, 23), Move(PlayerColor::WHITE, 24, 22)};
        auto playedMove = moves[0];
        auto turn = Turn(PlayerColor::WHITE, { 1, 2 }, moves, board);
        auto trie = TurnTrie({ turn }, board);

        // Act
        trie.playMove(playedMove);
        trie.undoMove();
        auto undoneBoard = trie.board();

        // Assert
        REQUIRE(board == undoneBoard);
    }
}
