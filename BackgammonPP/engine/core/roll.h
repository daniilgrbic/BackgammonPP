#pragma once

#include "die.h"
#include "playercolor.h"

#include <vector>

class Roll
{
public:
    Roll();

    Roll(PlayerColor onRoll, Die& first, Die& second);

    std::vector<int> dice() const;

    PlayerColor onRoll() const;

private:
    PlayerColor m_onRoll;
    std::vector<int> m_dice;
};
