#include <catch2/catch_test_macros.hpp>

#include "../engine/core/boardstate.h"

TEST_CASE("Board State construction", "[core]" ) {
    SECTION("Points should be empty when constructing BoardState") {
        // Arrange
        const auto bs = BoardState();

        // Assert
        REQUIRE(bs.point(1).count() == 0);
    }
}

TEST_CASE("Board State mirroring") {
    SECTION("Should mirror board for black player") {
        // Arrange
        auto bs = BoardState();
        bs.point(1).add(PlayerColor::WHITE, 4);
        bs.point(3).add(PlayerColor::BLACK, 3);
        bs.point(10).add(PlayerColor::WHITE, 1);
        bs.point(11).add(PlayerColor::BLACK, 2);
        bs.point(14).add(PlayerColor::WHITE, 5);
        bs.point(19).add(PlayerColor::BLACK, 6);

        // Act
        auto mbs = BoardState::centralMirror(bs);
        auto mmbs = BoardState::centralMirror(mbs);

        // Assert
        REQUIRE(bs == mmbs);
        REQUIRE(mbs.point(Point::centralMirrorId(1)) == Point{PlayerColor::WHITE, 4});
        REQUIRE(mbs.point(Point::centralMirrorId(3)) == Point{PlayerColor::BLACK, 3});
        REQUIRE(mbs.point(Point::centralMirrorId(10)) == Point{PlayerColor::WHITE, 1});
        REQUIRE(mbs.point(Point::centralMirrorId(11)) == Point{PlayerColor::BLACK, 2});
        REQUIRE(mbs.point(Point::centralMirrorId(14)) == Point{PlayerColor::WHITE, 5});
        REQUIRE(mbs.point(Point::centralMirrorId(19)) == Point{PlayerColor::BLACK, 6});
    }
}
