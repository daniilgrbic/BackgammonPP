#pragma once

#include "die.h"
#include "playercolor.h"

#include <vector>

class Roll
{
public:
    Roll();

    Roll(PlayerColor onRoll, Die& first, Die& second);

    Roll(PlayerColor onRoll, const std::vector<int>& dice);

    static Roll getInitialRoll(Die& first, Die& second);

    Roll getNextRoll(Die& first, Die& second) const;

    std::vector<int> dice() const;

    PlayerColor onRoll() const;

private:
    PlayerColor m_onRoll;
    std::vector<int> m_dice;
};
