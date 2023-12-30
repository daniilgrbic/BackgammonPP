#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include "../engine/longnardy.h"
#include "../engine/core/playercolor.h"

class LongNardyTestHelper : public LongNardy {
public:
    void setCurrentRoll(PlayerColor onRoll, const std::vector<int>& dice) {
        m_currentRoll = {onRoll, dice};
    }

    void setBoardState(BoardState board) {
        m_board = board;
    }

    void fillHistory() {
        // this is used to gaslight the turn generator into believing
        // it's not playing an initial turn
        m_history.resize(2);
    }
};

void printTurns(const std::vector<Turn>& turns) {
    for(const auto &turn : turns) {
        for(const auto &move : turn.m_moves) {
            if(std::holds_alternative<int>(move.m_from)) std::cout << std::get<int>(move.m_from);
            else std::cout << ((std::get<SpecialPosition>(move.m_from) == SpecialPosition::BAR) ? "BAR" : "OFF");
            std::cout << "->";
            if(std::holds_alternative<int>(move.m_to)) std::cout << std::get<int>(move.m_to);
            else std::cout << ((std::get<SpecialPosition>(move.m_to) == SpecialPosition::BAR) ? "BAR" : "OFF");
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

TEST_CASE("Long Nardy game construction") {

    SECTION("Should construct a game of Long Nardy") {
        for(int i = 0; i < 20; i++) {
            // Arrange & Act
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
}

TEST_CASE("Long Nardy first turn generation") {

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
}

TEST_CASE("Long Nardy prime rule") {

    SECTION("Should forbid forming a prime if no opponent checkers in front of it") {
        // Arrange
        BoardState board{};
        board.point(24).add(PlayerColor::WHITE, 9);
        board.point(9).add(PlayerColor::WHITE, 1);
        board.point(8).add(PlayerColor::WHITE, 1);
        board.point(7).add(PlayerColor::WHITE, 1);
        board.point(5).add(PlayerColor::WHITE, 1);
        board.point(4).add(PlayerColor::WHITE, 1);
        board.point(10).add(PlayerColor::WHITE, 1);
        board.point(Point::centralMirrorId(24)).add(PlayerColor::BLACK, 15);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {4, 1});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        int blocked;
        for(const auto& turn : turns) {
            blocked = 0;
            for(int p = 4; p <= 9; p++) {
                if(turn.m_finalBoard.point(p).owner().has_value())
                    blocked++;
            }
            REQUIRE(blocked < 6);

            blocked = 0;
            for(int p = 5; p <= 10; p++) {
                if(turn.m_finalBoard.point(p).owner().has_value())
                    blocked++;
            }
            REQUIRE(blocked < 6);
        }
    }

    SECTION("Should allow forming a prime if there are opponent checkers in front of it") {
        // Arrange
        BoardState board{};
        board.point(24).add(PlayerColor::WHITE, 9);
        board.point(9).add(PlayerColor::WHITE, 1);
        board.point(8).add(PlayerColor::WHITE, 1);
        board.point(7).add(PlayerColor::WHITE, 1);
        board.point(5).add(PlayerColor::WHITE, 1);
        board.point(4).add(PlayerColor::WHITE, 1);
        board.point(10).add(PlayerColor::WHITE, 1);
        board.point(Point::centralMirrorId(24)).add(PlayerColor::BLACK, 14);
        board.point(Point::centralMirrorId(13)).add(PlayerColor::BLACK, 1);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {4, 1});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        int blocked;
        bool primeFormed = false;
        for(const auto& turn : turns) {
            blocked = 0;
            for(int p = 4; p <= 9; p++) {
                if(turn.m_finalBoard.point(p).owner().has_value())
                    blocked++;
            }
            if(blocked >= 6) {
                primeFormed = true;
                break;
            }

            blocked = 0;
            for(int p = 5; p <= 10; p++) {
                if(turn.m_finalBoard.point(p).owner().has_value())
                    blocked++;
            }
            if(blocked >= 6) {
                primeFormed = true;
                break;
            }
        }
        REQUIRE(primeFormed);
    }
}

TEST_CASE("Long Nardy bearoff") {

    SECTION("Should properly bear off - impossible to remove both checkers") {
        // Arrange
        BoardState board{};
        board.point(4).add(PlayerColor::WHITE, 2);
        board.point(6).add(PlayerColor::WHITE, 2);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {5, 6});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        for(const auto& turn : turns) {
            REQUIRE(turn.m_finalBoard.point(1).count() == 1);
            REQUIRE(turn.m_finalBoard.point(4).count() == 2);
        }
    }

    SECTION("Should properly bear off - impossible to remove any checkers") {
        // Arrange
        BoardState board{};
        board.point(4).add(PlayerColor::WHITE, 2);
        board.point(6).add(PlayerColor::WHITE, 2);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {3, 5});
        game.setBoardState(board);

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        for(const auto& turn : turns) {
            for(const auto& move : turn.m_moves) {
                REQUIRE(std::holds_alternative<int>(move.m_to));
            }
        }
    }

    SECTION("Should properly bear off - exactly remove last 2 checkers") {
        // Arrange
        BoardState board{};
        board.point(2).add(PlayerColor::WHITE, 1);
        board.point(5).add(PlayerColor::WHITE, 1);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {2, 5});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        for(const auto& turn : turns) {
            REQUIRE((turn.m_finalBoard.point(2).count() == 1
                     or turn.m_finalBoard.off(PlayerColor::WHITE) == 2));
        }
    }

    SECTION("Should properly bear off - remove 2 checkers with higher rolls") {
        // Arrange
        BoardState board{};
        board.point(2).add(PlayerColor::WHITE, 1);
        board.point(3).add(PlayerColor::WHITE, 1);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {4, 5});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        for(const auto& turn : turns) {
            REQUIRE(turn.m_finalBoard.off(PlayerColor::WHITE) == 2);
        }
    }

    SECTION("Should properly bear off - remove 4 checkers with a double roll") {
        // Arrange
        BoardState board{};
        board.point(3).add(PlayerColor::WHITE, 5);

        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {3, 3, 3, 3});
        game.setBoardState(board);
        game.fillHistory();

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.front().m_finalBoard.point(3).count() == 1);
        REQUIRE(turns.front().m_finalBoard.off(PlayerColor::WHITE) == 4);
    }
}
