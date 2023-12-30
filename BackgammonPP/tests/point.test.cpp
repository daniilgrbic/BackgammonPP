#include <catch2/catch_test_macros.hpp>

#include "../engine/core/point.h"

TEST_CASE("Point class core functionality") {
    SECTION("Should construct unoccupied point") {
        // Arrange
        auto point = Point();

        // Assert
        REQUIRE(not point.owner().has_value());
        REQUIRE(point.count() == 0);
    }

    SECTION("Should place checkers of given color on an empty point") {
        // Arrange
        auto point = Point();

        // Act
        point.add(PlayerColor::BLACK, 5);

        // Assert
        REQUIRE(point.owner().has_value());
        REQUIRE(point.owner().value() == PlayerColor::BLACK);
        REQUIRE(point.count() == 5);
    }

    SECTION("Should not allow placing checkers of different colors on the same point") {
        // Arrange
        auto point = Point();
        point.add(PlayerColor::BLACK, 5);

        // Act + Assert
        REQUIRE_THROWS_AS(point.add(PlayerColor::WHITE, 2), std::logic_error);
    }

    SECTION("Should allow removing all checkers") {
        // Arrange
        auto point = Point();
        point.add(PlayerColor::WHITE, 3);

        // Act
        point.remove(3);

        // Assert
        REQUIRE(not point.owner().has_value());
        REQUIRE(point.count() == 0);
    }

    SECTION("Remove more than checker count") {
        // Arrange
        auto point = Point();
        point.add(PlayerColor::WHITE, 3);

        REQUIRE_THROWS_AS(point.remove(4), std::logic_error);
    }
}

TEST_CASE("Point class serialization") {
    SECTION("Should serialize empty point") {
        // Arrange
        auto point = Point();

        // Act
        QVariantMap serializedPointMap = point.toVariant().toMap();

        // Assert
        REQUIRE(serializedPointMap.size() == 1);
        REQUIRE(serializedPointMap.value("count").toInt() == 0);
    }

    SECTION("Should serialize occupied point") {
        // Arrange
        auto point = Point();
        point.add(PlayerColor::WHITE, 2);

        // Act
        QVariantMap serializedPointMap = point.toVariant().toMap();

        // Assert
        REQUIRE(serializedPointMap.size() == 2);
        REQUIRE(serializedPointMap.value("owner").toString().toStdString() == "white");
        REQUIRE(serializedPointMap.value("count").toInt() == 2);
    }

    SECTION("Should deserialize point") {
        // Arrange
        QVariantMap data;
        data.insert("owner", "white");
        data.insert("count", 4);

        // Act
        auto point = Point();
        point.fromVariant(data);

        // Assert
        REQUIRE(point.owner().has_value());
        REQUIRE(point.owner().value() == PlayerColor::WHITE);
        REQUIRE(point.count() == 4);
    }
}

TEST_CASE("Static methods"){
    SECTION("idByPlayer invalid index throws"){
        // Arrane
        PlayerColor color = PlayerColor::WHITE;
        int index = 25;

        // Act

        // Assert
        REQUIRE_THROWS(Point::idByPlayer(color, index));
    }
    SECTION("idByPlayer valid index doesnt throw"){
        // Arrane
        PlayerColor color = PlayerColor::WHITE;
        int index = 12;

        // Act

        // Assert
        REQUIRE_NOTHROW(Point::idByPlayer(color, index));
    }
    SECTION("idByPlayer white index 5"){
        // Arrane
        PlayerColor color = PlayerColor::WHITE;
        int index = 5;

        // Act
        int id = Point::idByPlayer(color, index);

        // Assert
        REQUIRE(5 == id);
    }
    SECTION("idByPlayer black index 16"){
        // Arrane
        PlayerColor color = PlayerColor::BLACK;
        int index = 16;

        // Act
        int id = Point::idByPlayer(color, index);

            // Assert
            REQUIRE(9 == id);
    }
    SECTION("centralMirrorId invalid index throws"){
        // Arrane
        int index = 25;

        // Act

        // Assert
        REQUIRE_THROWS(Point::centralMirrorId(index));
    }
    SECTION("centralMirrorId invalid index doesnt throw"){
        // Arrane
        int index = 12;

        // Act

        // Assert
        REQUIRE_NOTHROW(Point::centralMirrorId(index));
    }
    SECTION("centralMirrorId index 5"){
        // Arrane
        int index = 5;

        // Act
        int id = Point::centralMirrorId(index);

        // Assert
        REQUIRE(17 == id);
    }
    SECTION("centralMirrorId index 13"){
        // Arrane
        int index = 13;

        // Act
        int id = Point::centralMirrorId(index);

        // Assert
        REQUIRE(1 == id);
    }
    SECTION("verticalMirrorId invalid index throws"){
        // Arrane
        int index = 25;

        // Act

        // Assert
        REQUIRE_THROWS(Point::verticalMirrorId(index));
    }
    SECTION("verticalMirrorId invalid index doesnt throw"){
        // Arrane
        int index = 12;

        // Act

        // Assert
        REQUIRE_NOTHROW(Point::verticalMirrorId(index));
    }
    SECTION("verticalMirrorId index 5"){
        // Arrane
        int index = 5;

        // Act
        int id = Point::verticalMirrorId(index);

        // Assert
        REQUIRE(20 == id);
    }
}
