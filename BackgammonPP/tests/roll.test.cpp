#include <catch2/catch_test_macros.hpp>

#include "../engine/core/roll.h"
#include "../engine/core/die.h"

auto const WHI = PlayerColor::WHITE;
auto const BLA = PlayerColor::BLACK;

TEST_CASE("Roll serialization" ) {
    SECTION("Should deserialize a roll") {
        // Arrange
        Die first, second;
        first.roll();
        second.roll();
        Roll roll(WHI, first, second);

        // Act
        QVariantMap qMap = roll.toVariant().toMap();
        Roll roll_deserialized;
        roll_deserialized.fromVariant(qMap);

        // Assert
        REQUIRE(roll == roll_deserialized);
    }

    SECTION("Should be 4 dice if all are equal, otherwise two "){
        //ARRANGE
        Die a, b;
        int NO_OF_TEST_ITERS = 100;

        for(int i = 0; i < NO_OF_TEST_ITERS; i++){
            //ACT
            Roll roll(WHI, a, b);

            //ASSERT
            auto dice = roll.dice();
            bool cond = (dice.size() == 2 || dice.size() == 4);
            REQUIRE(cond);
            if(dice.size() == 2){
                REQUIRE((dice[0] != dice[1]));
            }else{
                REQUIRE((dice[0] == dice[1] && dice[1] == dice[2] && dice[2] == dice[3]));
            }
        }
    }

}

