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

Roll::Roll(PlayerColor onRoll, const std::vector<int>& dice)
    : m_onRoll { onRoll }, m_dice { dice }
{}

Roll Roll::getInitialRoll(Die& first, Die& second) {
    int firstValue, secondValue;
    do {
        firstValue = first.roll();
        secondValue = second.roll();
    } while (firstValue == secondValue);

    auto onRoll = firstValue > secondValue ? PlayerColor::WHITE : PlayerColor::BLACK;
    return { onRoll, { firstValue, secondValue } };
}

Roll Roll::getNextRoll(Die& first, Die& second) const {
    auto opponent = m_onRoll == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE;
    return { opponent, first, second };
}

std::vector<int> Roll::dice() const {
    return m_dice;
}

PlayerColor Roll::onRoll() const {
    return m_onRoll;
}
