#include "roll.h"

Roll::Roll()
{
}

Roll::Roll(PlayerColor onRoll, Die& first, Die& second)
    : m_onRoll { onRoll }
{
    auto firstValue = first.roll();
    auto secondValue = second.roll();
    if (firstValue != secondValue) {
        m_dice = { firstValue, secondValue };
    } else {
        m_dice = std::vector<int>(4, firstValue);
    }
}

std::vector<int> Roll::dice() const {
    return m_dice;
}

PlayerColor Roll::onRoll() const {
    return m_onRoll;
}
