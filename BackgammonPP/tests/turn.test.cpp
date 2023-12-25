#include <catch2/catch_test_macros.hpp>

#include "../engine/core/turn.h"

auto const WHI = PlayerColor::WHITE;
auto const BLA = PlayerColor::BLACK;
auto const BAR = SpecialPosition::BAR;
auto const OFF = SpecialPosition::OFF;

TEST_CASE("Turn struct serialization") {
    SECTION("Should serialize a turn") {
        // Arrange
        Turn turn {1, WHI, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};

        // Act
        QVariantMap qMap = turn.toVariant().toMap();

        // Assert
        REQUIRE(qMap.size() == 4);
        REQUIRE(qMap.value("index").toInt() == 1);
        REQUIRE(qMap.value("player").toString().toStdString() == "white");
        REQUIRE(qMap.value("moves").toList().size() == 2);
    }

    SECTION("Should deserialize a turn") {
        // Arrange
        QVariantMap data;
        data.insert("index", 1);
        data.insert("player", "white");
        data.insert("moves", QVariantList{Move{WHI, BAR, 20}.toVariant(), Move{WHI, 20, 19}.toVariant()});
        data.insert("finalBoard", BoardState{}.toVariant());

        // Act
        Turn turn{};
        turn.fromVariant(data);

        // Assert
        REQUIRE(turn.m_index == 1);
        REQUIRE(turn.m_player == PlayerColor::WHITE);
        REQUIRE(turn.m_moves.size() == 2);
    }
}
