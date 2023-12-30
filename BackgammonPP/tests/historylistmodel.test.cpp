#include <catch2/catch_test_macros.hpp>

#include "../historylistmodel.h"
#include "../engine/core/turn.h"

auto const WHI = PlayerColor::WHITE;
auto const BLA = PlayerColor::BLACK;
auto const BAR = SpecialPosition::BAR;
auto const OFF = SpecialPosition::OFF;

TEST_CASE("History list model class"){
    SECTION("Should construct History with empty QList"){
        // Arrange
        HistoryListModel model;

        // Act


        // Assert
        REQUIRE(model.rowCount() == 0);

    }
    SECTION("Should add turn to list"){
        // Arrange
        HistoryListModel model;

        // Act
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};
        model.addTurn(turn);

        // Assert
        REQUIRE(model.rowCount() == 1);
    }

    SECTION("Should get first string"){
        // Arrange
        HistoryListModel model;
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};
        model.addTurn(turn);

        // Act
        QModelIndex index = model.index(0, 0);
        QVariant value = model.data(index, Qt::DisplayRole);

        // Assert
        REQUIRE(value.toString() == "W 5-1: bar/20 20/19");
    }
    SECTION("Should return non valid qvariant"){
        // Arrange
        HistoryListModel model;

        // Act
        QModelIndex index = model.index(5, 0);
        QVariant value = model.data(index, Qt::DisplayRole);

        // Assert
        REQUIRE_FALSE(value.isValid());
    }
    SECTION("Should return turn at index"){
        // Arrange
        HistoryListModel model;
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};
        model.addTurn(turn);

        // Act
        QModelIndex index = model.index(0, 0);
        QVariant value = model.data(index, Qt::UserRole);
        Turn turn2;
        turn2.fromVariant(value);

        // Assert
        REQUIRE(turn2 == turn);
    }
    SECTION("Should return true if turn is last turn"){
        // Arrange
        HistoryListModel model;
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};
        model.addTurn(turn);
        model.addTurn(turn);
        model.addTurn(turn);

        // Act
        QModelIndex index = model.index(2, 0);
        QVariant value = model.data(index, Qt::UserRole + 1);
        bool lastTurn = value.toBool();


        // Assert
        REQUIRE(lastTurn);
    }
    SECTION("Should return false if turn is not last turn"){
        // Arrange
        HistoryListModel model;
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};
        model.addTurn(turn);
        model.addTurn(turn);
        model.addTurn(turn);

        // Act
        QModelIndex index = model.index(0, 0);
        QVariant value = model.data(index, Qt::UserRole + 1);
        bool lastTurn = value.toBool();


        // Assert
        REQUIRE_FALSE(lastTurn);
    }
    SECTION("Should return elements count"){
        // Arrange
        HistoryListModel model;
        Turn turn {WHI, {5, 1}, {{WHI, BAR, 20}, {WHI, 20, 19}}, {}};


        // Act
        model.addTurn(turn);
        model.addTurn(turn);
        model.addTurn(turn);


        // Assert
        REQUIRE(model.rowCount() == 3);
    }
}
