#include <catch2/catch_test_macros.hpp>

#include "stringlistmodel.h"

TEST_CASE("String list model class"){
    SECTION("Should construct StringListModel from QStringList"){
        // Arrange
        QStringList list {"one", "two", "three"};

        // Act
        StringListModel model(list);

        // Assert
        REQUIRE(model.rowCount() == 3);

    }
    SECTION("Should add one opponent to list"){
        // Arrange
        QStringList list;
        StringListModel model(list);

        // Act
        model.addOpponent("one");

        // Assert
        REQUIRE(model.rowCount() == 1);
    }
    SECTION("Should remove one opponent from list"){
        // Arrange
        QStringList list {"one", "two"};
        StringListModel model(list);

        // Act
        model.removeOpponent("one");

        // Assert
        REQUIRE(model.rowCount() == 1);
    }
    SECTION("Should get first string"){
        // Arrange
        QStringList list {"one", "two", "three"};
        StringListModel model(list);

        // Act
        QModelIndex index = model.index(0, 0);
        QVariant value = model.data(index, Qt::DisplayRole);

        // Assert
        REQUIRE(value.toString() == "one");
    }
    SECTION("Should return non valid qvariant"){
        // Arrange
        QStringList list {"one", "two", "three"};
        StringListModel model(list);

        // Act
        QModelIndex index = model.index(5, 0);
        QVariant value = model.data(index, Qt::DisplayRole);

        // Assert
        REQUIRE_FALSE(value.isValid());
    }
}
