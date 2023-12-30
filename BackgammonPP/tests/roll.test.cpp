#include <catch2/catch_test_macros.hpp>

#include "../engine/core/roll.h"
#include "../engine/core/die.h"

auto const WHI = PlayerColor::WHITE;
auto const BLA = PlayerColor::BLACK;

TEST_CASE("Roll serialization" ) {
    SECTION("Should deserialize a roll") {
        // Arrange
        Die first, second;
        Roll roll(WHI, first, second);

        // Act
        QVariantMap qMap = roll.toVariant().toMap();
        Roll roll_deserialized;
        roll_deserialized.fromVariant(qMap);

        // Assert
        REQUIRE(roll == roll_deserialized);
    }
}

TEST_CASE("First roll") {
    SECTION("Assert that initial rolls are different") {
        for (int i = 0; i < 10000; i++) {
            // Arrange
            Die first, second;
            Roll initialRoll = Roll::getInitialRoll(first, second);

            // Assert
            REQUIRE(first != second);
        }
    }
}
