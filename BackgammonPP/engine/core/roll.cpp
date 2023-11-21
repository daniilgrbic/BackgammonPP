#include "roll.h"

Roll::Roll()
{
}

Roll::Roll(PlayerColor onRoll, Die& first, Die& second)
    : onRoll { onRoll }
{
    auto firstValue = first.roll();
    auto secondValue = second.roll();
    if (firstValue != secondValue) {
        diceValue = { firstValue, secondValue };
    } else {
        diceValue = std::vector<int>(4, firstValue);
    }
}

std::vector<int> Roll::dice() const {
    return diceValue;
}

PlayerColor Roll::whosOnRoll() const {
    return onRoll;
}
