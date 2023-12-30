#include <catch2/catch_test_macros.hpp>

#include "../engine/core/die.h"



TEST_CASE("Die validation") {
    SECTION("Assert that roll is in [1, 6]") {
        for (int i = 0; i < 10000; i++) {
            // Arrange
            Die d;

            //Act
            int roll = d.roll();

            // Assert
            bool cond = 1 <= roll && roll <= 6;
            REQUIRE(cond);
        }
    }

}
