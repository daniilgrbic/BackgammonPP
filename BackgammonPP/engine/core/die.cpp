#include "die.h"

auto Die::roll() -> int {
    return m_die(m_mt);
}

auto Die::operator ==(const Die& other) const -> bool {
    return m_value == other.m_value;
}
