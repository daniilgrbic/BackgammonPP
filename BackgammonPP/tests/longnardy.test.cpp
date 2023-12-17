#include <catch2/catch_test_macros.hpp>

#include "../engine/core/boardstate.h"

TEST_CASE( "Long Nardy turn generation", "[class][ruleset]" ) {
    SECTION("First move") {
        // Arrange
        const auto bs = BoardState();

        // Assert
        REQUIRE(bs.point(1).count() == 0);
    }
}
