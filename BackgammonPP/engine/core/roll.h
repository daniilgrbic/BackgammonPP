#ifndef ROLL_H
#define ROLL_H

#include "die.h"
#include "playercolor.h"

#include <vector>

class Roll
{
public:
    Roll();

    Roll(PlayerColor onRoll, Die& first, Die& second);

    std::vector<int> dice() const;

    PlayerColor whosOnRoll() const;

private:
    PlayerColor onRoll;
    std::vector<int> diceValue;
};

#endif // ROLL_H
