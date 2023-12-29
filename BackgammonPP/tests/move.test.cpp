#include <catch2/catch_test_macros.hpp>

#include "../engine/core/move.h"

TEST_CASE("Move struct serialization") {
    SECTION("Should serialize move without special positions") {
        // Arrange
        Move move {PlayerColor::WHITE, 6, 2, true};

        // Act
        QVariantMap serializedMoveQMap = move.toVariant().toMap();

        // Assert
        REQUIRE(serializedMoveQMap.value("player").toString().toStdString() == "white");
        REQUIRE(serializedMoveQMap.value("isHit").toBool() == true);
        REQUIRE(serializedMoveQMap.value("from").toInt() == 6);
        REQUIRE(serializedMoveQMap.value("to").toInt() == 2);
    }

    SECTION("Should serialize move with special positions") {
        // Arrange
        Move move {PlayerColor::BLACK, 2, SpecialPosition::OFF};

        // Act
        QVariantMap serializedMoveQMap = move.toVariant().toMap();

        // Assert
        REQUIRE(serializedMoveQMap.value("player").toString().toStdString() == "black");
        REQUIRE(serializedMoveQMap.value("isHit").toBool() == false);
        REQUIRE(serializedMoveQMap.value("from").toInt() == 2);
        REQUIRE(serializedMoveQMap.value("to").toString().toStdString() == "off");
    }

    SECTION("Should deserialize move without special positions") {
        // Arrange
        QVariantMap data;
        data.insert("player", "white");
        data.insert("isHit", false);
        data.insert("from", 24);
        data.insert("to", 18);

        // Act
        Move move;
        move.fromVariant(data);

        // Assert
        REQUIRE(move.m_player == PlayerColor::WHITE);
        REQUIRE(move.m_isHit == false);
        REQUIRE(std::holds_alternative<int>(move.m_from));
        REQUIRE(std::get<int>(move.m_from) == 24);
        REQUIRE(std::holds_alternative<int>(move.m_to));
        REQUIRE(std::get<int>(move.m_to) == 18);
    }

    SECTION("Should deserialize move with special positions") {
        // Arrange
        QVariantMap data;
        data.insert("player", "white");
        data.insert("from", "bar");
        data.insert("to", 20);
        data.insert("isHit", true);

        // Act
        Move move;
        move.fromVariant(data);

        // Assert
        REQUIRE(move.m_player == PlayerColor::WHITE);
        REQUIRE(move.m_isHit == true);
        REQUIRE(std::holds_alternative<SpecialPosition>(move.m_from));
        REQUIRE(std::get<SpecialPosition>(move.m_from) == SpecialPosition::BAR);
        REQUIRE(std::holds_alternative<int>(move.m_to));
        REQUIRE(std::get<int>(move.m_to) == 20);
    }
}
