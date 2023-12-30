#include <catch2/catch_test_macros.hpp>

#include "../engine/core/boardstate.h"

TEST_CASE("Board State construction", "[core]" ) {
    SECTION("Points should be empty when constructing BoardState") {
        // Arrange
        const auto bs = BoardState();

        // Assert
        REQUIRE(bs.point(1).count() == 0);
    }

    SECTION("Non-empty point construction of BoardState") {
        // Arrange
        const auto bs = BoardState(
            {{1, 3}, {2, 2}},
            {{19, 4}, {21, 1}},
            2, 3,
            0, 1
        );

        // Assert
        REQUIRE(bs.point(1).count() == 3);
        REQUIRE(bs.point(2).count() == 2);
        REQUIRE(bs.point(19).count() == 4);
        REQUIRE(bs.point(21).count() == 1);

        REQUIRE(bs.bar(PlayerColor::WHITE) == 2);
        REQUIRE(bs.off(PlayerColor::WHITE) == 0);
        REQUIRE(bs.bar(PlayerColor::BLACK) == 3);
        REQUIRE(bs.off(PlayerColor::BLACK) == 1);
    }
}

TEST_CASE("Board State mirroring") {
    SECTION("Should mirror BoardState") {
        // Arrange
        auto bs = BoardState();
        bs.point(1).add(PlayerColor::WHITE, 4);
        bs.point(3).add(PlayerColor::BLACK, 3);

        // Act
        auto mbs = bs.mirror();
        auto mmbs = mbs.mirror();

        // Assert
        REQUIRE(bs == mmbs);
    }

    SECTION("Should do a central mirror board for black player") {
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

    SECTION("Should do a vertical mirror board for black player") {
        // Arrange
        auto bs = BoardState();
        bs.point(1).add(PlayerColor::WHITE, 4);
        bs.point(3).add(PlayerColor::BLACK, 3);
        bs.point(10).add(PlayerColor::WHITE, 1);
        bs.point(11).add(PlayerColor::BLACK, 2);
        bs.point(14).add(PlayerColor::WHITE, 5);
        bs.point(19).add(PlayerColor::BLACK, 6);

        // Act
        auto mbs = BoardState::verticalMirror(bs);
        auto mmbs = BoardState::verticalMirror(mbs);

        // Assert
        REQUIRE(bs == mmbs);
        REQUIRE(mbs.point(Point::verticalMirrorId(1)) == Point{PlayerColor::WHITE, 4});
        REQUIRE(mbs.point(Point::verticalMirrorId(3)) == Point{PlayerColor::BLACK, 3});
        REQUIRE(mbs.point(Point::verticalMirrorId(10)) == Point{PlayerColor::WHITE, 1});
        REQUIRE(mbs.point(Point::verticalMirrorId(11)) == Point{PlayerColor::BLACK, 2});
        REQUIRE(mbs.point(Point::verticalMirrorId(14)) == Point{PlayerColor::WHITE, 5});
        REQUIRE(mbs.point(Point::verticalMirrorId(19)) == Point{PlayerColor::BLACK, 6});
    }
}

TEST_CASE("Board State moving") {
    SECTION("Board State simple moving") {
        // Arrange
        auto bs = BoardState(
            {{1, 3}, {2, 2}},
            {{19, 4}, {21, 1}},
            2, 3,
            0, 1
        );

        const auto move1 = Move(
            PlayerColor::WHITE,
            1,
            2
        );

        // Act
        bs.move(move1);

        // Assert
        auto bsVariant = bs.toVariant().toMap();
        QVariantList bsVariantList = bsVariant.value("points").toList();
        REQUIRE(bsVariantList.size() == 24);
        REQUIRE(bsVariantList[0].toMap().value("owner") == "white");
        REQUIRE(bsVariantList[1].toMap().value("owner") == "white");
        REQUIRE(bsVariantList[0].toMap().value("count").toInt() == 2);
        REQUIRE(bsVariantList[1].toMap().value("count").toInt() == 3);
    }

    SECTION("Board State special position moving") {
        // Arrange
        auto bs = BoardState(
            {{1, 3}, {2, 2}},
            {{19, 4}, {21, 1}},
            2, 3,
            0, 1
            );

        const auto move1 = Move(
            PlayerColor::WHITE,
            1,
            SpecialPosition::OFF
            );

        // Act
        bs.move(move1);

        // Assert
        auto bsVariant = bs.toVariant().toMap();
        QVariantList bsVariantList = bsVariant.value("points").toList();
        REQUIRE(bsVariantList.size() == 24);
        REQUIRE(bsVariant.value("whiteOff").toInt() == 1);
        REQUIRE(bsVariantList[0].toMap().value("owner") == "white");
        REQUIRE(bsVariantList[0].toMap().value("count").toInt() == 2);
    }
}

TEST_CASE("Board State serialization") {
    SECTION("Board State to variant") {
        // Arrange
        const auto bs = BoardState(
            {{1, 3}, {2, 2}},
            {{19, 4}, {21, 1}},
            2, 3,
            0, 1
        );

        // Act
        auto bsVariant = bs.toVariant().toMap();

        // Assert
        REQUIRE(bsVariant.value("whiteBar").toInt() == 2);
        REQUIRE(bsVariant.value("blackBar").toInt() == 3);
        REQUIRE(bsVariant.value("whiteOff").toInt() == 0);
        REQUIRE(bsVariant.value("blackOff").toInt() == 1);
        QVariantList bsVariantList = bsVariant.value("points").toList();

        REQUIRE(bsVariantList.size() == 24);
        REQUIRE(bsVariantList[0].toMap().value("owner") == "white");
        REQUIRE(bsVariantList[0].toMap().value("count").toInt() == 3);
        REQUIRE(bsVariantList[1].toMap().value("owner") == "white");
        REQUIRE(bsVariantList[1].toMap().value("count").toInt() == 2);
        REQUIRE(bsVariantList[18].toMap().value("owner") == "black");
        REQUIRE(bsVariantList[18].toMap().value("count").toInt() == 4);
        REQUIRE(bsVariantList[20].toMap().value("owner") == "black");
        REQUIRE(bsVariantList[20].toMap().value("count").toInt() == 1);

        REQUIRE(bsVariantList[23].toMap().value("count").toInt() == 0);
    }
}
