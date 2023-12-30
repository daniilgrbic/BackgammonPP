#include <catch2/catch_test_macros.hpp>

#include "../engine/core/turn.h"

auto const WHI = PlayerColor::WHITE;
auto const BLA = PlayerColor::BLACK;
auto const BAR = SpecialPosition::BAR;
auto const OFF = SpecialPosition::OFF;

TEST_CASE("Turn struct serialization") {
    SECTION("Should serialize a turn") {
        // Arrange
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};

        // Act
        QVariantMap qMap = turn.toVariant().toMap();

        // Assert
        REQUIRE(qMap.size() == 4);
        REQUIRE(qMap.value("player").toString().toStdString() == "white");
        REQUIRE(qMap.value("moves").toList().size() == 2);
        REQUIRE(qMap.value("dice").toList().size() == 2);
    }

    SECTION("Should deserialize a turn") {
        // Arrange
        QVariantMap data;
        data.insert("player", "white");
        data.insert("dice", QVariantList{5, 1});
        data.insert("moves", QVariantList{Move{WHI, BAR, 20}.toVariant(), Move{WHI, 20, 19}.toVariant()});
        data.insert("finalBoard", BoardState{}.toVariant());

        // Act
        Turn turn{};
        turn.fromVariant(data);

        // Assert
        REQUIRE(turn.m_player == PlayerColor::WHITE);
        REQUIRE(turn.m_dice == std::vector<int>{5, 1});
        REQUIRE(turn.m_moves.size() == 2);
    }

    SECTION("Should return unique moves"){
        // Arrange
        BoardState state({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(24, 2)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );
        Turn turn1 {BLA, {2, 1}, {{BLA, 1, 2}, {BLA, 2, 4}}, {}};
        Turn turn2 {BLA, {2, 1}, {{BLA, 1, 3}, {BLA, 3, 4}}, {}};
        BoardState board1 = state.getNextState(turn1.m_moves);
        turn1.m_finalBoard = board1;
        BoardState board2 = state.getNextState(turn2.m_moves);
        turn2.m_finalBoard = board2;

        // Act
        std::vector<Turn> turns = Turn::unique({turn1, turn2});

        // Assert
        REQUIRE(turns.size() == 1);
    }

    SECTION("Should not merge moves"){
        // Arrange
        BoardState state({std::make_pair(6, 5), std::make_pair(8, 3), std::make_pair(13, 5), std::make_pair(24, 1), std::make_pair(2, 1)},
                         {std::make_pair(1, 2), std::make_pair(12, 5), std::make_pair(17, 3), std::make_pair(19, 5)}
                         ,0 ,0 ,0 ,0 );
        Turn turn1 {BLA, {2, 1}, {{BLA, 1, 2, true}, {BLA, 2, 4}}, {}};
        Turn turn2 {BLA, {2, 1}, {{BLA, 1, 3}, {BLA, 3, 4}}, {}};
        BoardState board1 = state.getNextState(turn1.m_moves);
        turn1.m_finalBoard = board1;
        BoardState board2 = state.getNextState(turn2.m_moves);
        turn2.m_finalBoard = board2;


        // Act
        std::vector<Turn> turns = Turn::unique({turn1, turn2});

        // Assert
        REQUIRE_FALSE(turns.size() == 1);
    }
}
