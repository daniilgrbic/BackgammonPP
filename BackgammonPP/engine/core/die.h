#pragma once

#include <random>

class Die
{
public:    
    int roll();
    bool operator ==(const Die& other) const;

private:
    int m_value;
    std::mt19937 m_mt { std::random_device {}()};
    std::uniform_int_distribution<> m_die { 1, 6 };
};
