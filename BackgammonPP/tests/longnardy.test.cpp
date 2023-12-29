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
        REQUIRE(turns.size() == 2); // turns 24/22/19 and 24/21/19
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
        REQUIRE(turns.size() == 1); // turn 24/22/20/18/16
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 14);
        REQUIRE(finalBoard.point(16).count() == 1);
    }

    SECTION("Should generate legal turns for white player - first roll double 3s") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {3, 3, 3, 3});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(turns.size() == 1);
        REQUIRE(finalBoard.point(24).count() == 13);
        REQUIRE(finalBoard.point(21).count() == 1);
        REQUIRE(finalBoard.point(15).count() == 1);
    }

    SECTION("Should generate legal turns for white player - first roll double 4s") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {4, 4, 4, 4});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(turns.size() == 1);
        REQUIRE(finalBoard.point(24).count() == 13);
        REQUIRE(finalBoard.point(16).count() == 2);
    }

    SECTION("Should generate legal turns for black player - first game roll double 4 after white rolls double 5") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {5, 5, 5, 5});
        auto turns = game.generateLegalTurns(); // single turn 24/19/14/9/4
        game.playTurn(turns.front());
        game.setCurrentRoll(PlayerColor::BLACK, {4, 4, 4, 4});

        // Act
        turns = game.generateLegalTurns();

        // Assert
        auto finalBoard = turns.front().m_finalBoard;
        REQUIRE(turns.size() == 1);
        REQUIRE(finalBoard.point(24) == Point{PlayerColor::WHITE, 14});
        REQUIRE(finalBoard.point(4) == Point{PlayerColor::WHITE, 1});
        REQUIRE(finalBoard.point(Point::centralMirrorId(24)) == Point{PlayerColor::BLACK, 14});
        REQUIRE(finalBoard.point(Point::centralMirrorId(20)) == Point{PlayerColor::BLACK, 1});
    }

    SECTION("Should generate legal turns for white player - first roll double 6s") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {6, 6, 6, 6});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
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
        bs.point(14).add(PlayerColor::WHITE, 5);
        bs.point(19).add(PlayerColor::BLACK, 6);

        // Act
        auto mbs = BoardState::centralMirror(bs);
        auto mmbs = BoardState::centralMirror(mbs);

        // Assert
        REQUIRE(bs == mmbs);
        REQUIRE(mbs.point(Point::centralMirrorId(1)) == Point{PlayerColor::WHITE, 4});
        REQUIRE(mbs.point(Point::centralMirrorId(3)) == Point{PlayerColor::BLACK, 3});
        REQUIRE(mbs.point(Point::centralMirrorId(10)) == Point{PlayerColor::WHITE, 1});
        REQUIRE(mbs.point(Point::centralMirrorId(11)) == Point{PlayerColor::BLACK, 2});
        REQUIRE(mbs.point(Point::centralMirrorId(14)) == Point{PlayerColor::WHITE, 5});
        REQUIRE(mbs.point(Point::centralMirrorId(19)) == Point{PlayerColor::BLACK, 6});
    }
}
