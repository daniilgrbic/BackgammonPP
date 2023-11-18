#include "die.h"

int Die::roll() {
    return die(mt);
}

bool Die::operator ==(const Die& other) const {
    return value == other.value;
}
