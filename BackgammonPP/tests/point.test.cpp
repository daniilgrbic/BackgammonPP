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
