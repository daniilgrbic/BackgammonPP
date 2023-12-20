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
    SECTION("Constructor") {

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

    SECTION("First roll 2 and 3") {
        // Arrange
        auto game = LongNardyTestHelper();
        game.setCurrentRoll(PlayerColor::WHITE, {2, 3});

        // Act
        auto turns = game.generateLegalTurns();

        // Assert
        REQUIRE(turns.size() == 1);
        auto finalBoard = turns[0].m_finalBoard;
        REQUIRE(finalBoard.point(24).count() == 14);
        REQUIRE(finalBoard.point(19).count() == 1);
    }
}
