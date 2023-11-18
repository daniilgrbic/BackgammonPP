#ifndef DIE_H
#define DIE_H

#include <random>

class Die
{
public:    
    int roll();

    bool operator ==(const Die& other) const;

private:
    int value;

    std::mt19937 mt { std::random_device {}()};
    std::uniform_int_distribution<> die { 1, 6 };
};

#endif // DIE_H
