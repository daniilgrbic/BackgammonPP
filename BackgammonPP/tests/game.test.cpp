#include <catch2/catch_test_macros.hpp>

#include "../engine/backgammon.h"
#include <utility>

class BackgammonTestHelper : public Backgammon {
public:
    void setBoard(const BoardState& board) {
        m_board = board;
    }

    void setCurrentRoll(PlayerColor onRoll, const std::vector<int>& dice) {
        m_currentRoll = {onRoll, dice};
    }

    void setCurrentRoll(const Roll& roll) {
        m_currentRoll = roll;
    }
};

TEST_CASE("Game core functionality"){
    SECTION("White should be on roll after black"){
        // Arrange
        BoardState state;
        Roll roll(PlayerColor::WHITE, {1, 2});
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(state);
        game.setCurrentRoll(roll);
        std::vector<Turn> turns = game.generateLegalTurns();
        game.playTurn(turns[0]);
        PlayerColor nextPlayer = game.currentRoll().onRoll();

        // Assert
        REQUIRE(nextPlayer == PlayerColor::BLACK);
    }
    SECTION("White should be on roll"){
        // Arrange
        BoardState state;
        Roll roll(PlayerColor::WHITE, {1, 2});
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(state);
        game.setCurrentRoll(roll);
        PlayerColor nextPlayer = game.currentRoll().onRoll();

        // Assert
        REQUIRE(nextPlayer == PlayerColor::WHITE);
    }
    SECTION("Should return starting board"){
        // Arrange
        BoardState state({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(24, 2)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );
        Roll roll(PlayerColor::WHITE, {1, 2});
        auto game = BackgammonTestHelper();

        // Act
        game.setBoard(state);
        game.setCurrentRoll(roll);
        BoardState gameState = game.board();

        // Assert
        REQUIRE(gameState == state);

    }
    SECTION("Should return board after white rolls 1,2 and plays 24/23 24/22"){
        // Arrange
        BoardState state({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(24, 2)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );
        Roll roll(PlayerColor::WHITE, {1, 2});
        auto game = BackgammonTestHelper();
        BoardState nextState({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(23, 1),std::make_pair(22, 1)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );

        // Act
        game.setBoard(state);
        game.setCurrentRoll(roll);
        std::vector<Turn> turns = game.generateLegalTurns();
        Turn turn;
        for(auto t : turns){
            if(t.m_finalBoard == nextState)
                turn = t;
        }
        game.playTurn(turn);
        BoardState newState = game.board();

        // Assert
        REQUIRE_FALSE(turn.m_moves.empty());
        REQUIRE(newState == nextState);
    }
}
