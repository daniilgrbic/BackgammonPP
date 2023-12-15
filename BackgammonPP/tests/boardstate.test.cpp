#include <catch2/catch_test_macros.hpp>

#include "../engine/core/boardstate.h"

TEST_CASE( "BoardState", "[core]" ) {
    SECTION("Points should be empty when constructing BoardState") {
        // Arrange
        const auto bs = BoardState();

        // Assert
        REQUIRE(bs.point(1).count() == 0);
    }
}
