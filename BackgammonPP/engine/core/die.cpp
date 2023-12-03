#include "die.h"

int Die::roll() {
    return m_die(m_mt);
}

bool Die::operator ==(const Die& other) const {
    return m_value == other.m_value;
}
