#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include "../engine/longnardy.h"
#include "../engine/core/playercolor.h"

class LongNardyTestHelper : public LongNardy {
public:
    void setCurrentRoll(PlayerColor onRoll, const std::vector<int>& dice) {
        m_currentRoll = {onRoll, dice};
    }
};

void printTurns(const std::vector<Turn>& turns) {
    for(const auto &turn : turns) {
        for(const auto &move : turn.m_moves) {
            std::cout << std::get<int>(move.m_from) << "->" << std::get<int>(move.m_to) << " ";
        }
        std::cout << std::endl;
    }
}

TEST_CASE( "Long Nardy turn generation", "[class][ruleset]" ) {
    SECTION("Should construct a game of Long Nardy") {

        for(int i = 0; i < 20; i++) {
            // Arrange
            auto game = LongNardyTestHelper();
            auto onRoll = game.currentRoll().onRoll();
            auto dice = game.currentRoll().dice();

            // Assert
            REQUIRE((onRoll == PlayerColor::WHITE or onRoll == PlayerColor::BLACK));
            REQUIRE((dice.size() == 2 or dice.size() == 4));
            if(dice.size() == 2) {
                REQUIRE((dice[0] != dice[1]));
            }
            else {
                REQUIRE((dice[0] == dice[1] and dice[1] == dice[2] and dice[2] == dice[3]));
            }
        }
    }

    SECTION("Should generate legal turns for white player - first roll 2 and 3") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {2, 3});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 14);
        REQUIRE(finalBoard.point(19).count() == 1);
    }

    SECTION("Should generate legal turns for white player - first roll a double (other than 4s or 6s)") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {2, 2, 2, 2});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 14);
        REQUIRE(finalBoard.point(16).count() == 1);
    }

    SECTION("Should generate legal turns for white player - first roll double 4s") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {4, 4, 4, 4});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 13);
        REQUIRE(finalBoard.point(16).count() == 2);
    }

    SECTION("Should generate legal turns for white player - first roll double 6s") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {6, 6, 6, 6});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 13);
        REQUIRE(finalBoard.point(18).count() == 2);
    }

    SECTION("Should mirror board for black player") {
        // Arrange
        auto bs = BoardState();
        bs.point(1).add(PlayerColor::WHITE, 4);
        bs.point(3).add(PlayerColor::BLACK, 3);
        bs.point(10).add(PlayerColor::WHITE, 1);
        bs.point(11).add(PlayerColor::BLACK, 2);
        bs.point(12).add(PlayerColor::WHITE, 5);
        bs.point(13).add(PlayerColor::BLACK, 6);

        // Act
        auto mbs = BoardState::centralMirror(bs);
        auto mmbs = BoardState::centralMirror(mbs);

        // Assert
        REQUIRE(bs == mmbs);
        REQUIRE((mbs.point(13).count() == 4 and mbs.point(13).owner() == PlayerColor::WHITE));
        REQUIRE((mbs.point(15).count() == 3 and mbs.point(15).owner() == PlayerColor::BLACK));
        REQUIRE((mbs.point(22).count() == 1 and mbs.point(22).owner() == PlayerColor::WHITE));
        REQUIRE((mbs.point(23).count() == 2 and mbs.point(23).owner() == PlayerColor::BLACK));
        REQUIRE((mbs.point(24).count() == 5 and mbs.point(24).owner() == PlayerColor::WHITE));
        REQUIRE((mbs.point(1).count() == 6 and mbs.point(1).owner() == PlayerColor::BLACK));
    }

    SECTION("Should generate legal turns for black player - first roll 2 and 3") {
        // Arrange
        auto game = LongNardyTestHelper();
        auto player = PlayerColor::BLACK;
        game.setCurrentRoll(player, {2, 3});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(12).count() == 14);
        REQUIRE(finalBoard.point(7).count() == 1);
    }
}
