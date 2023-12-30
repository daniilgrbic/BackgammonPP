#include <catch2/catch_test_macros.hpp>

#include "../engine/backgammon.h"
#include <utility>

TEST_CASE("Game core functionality"){
    SECTION("White should be on roll after black"){
        // Arrange
        BoardState state;
        Roll roll(PlayerColor::WHITE, {1, 2});
        Backgammon game(state, roll);


        // Act
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
        Backgammon game(state, roll);


        // Act
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
        Backgammon game(state, roll);

        // Act
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
        Backgammon game(state, roll);
        BoardState nextState({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(23, 1),std::make_pair(22, 1)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );

        // Act
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
