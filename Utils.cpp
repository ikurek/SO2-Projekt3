#include <iostream>
#include "Utils.h"

using namespace std;

// Calculates a random int number in rangoe [a, b]
int Utils::randomInRange(int start, int end)
{

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(start, end);

    return dist(mt);
}

// User uniform distribution to perform a simple coin flip test
// Returns value a, or b
// With 50% chance
int Utils::randomCoinFlip(int heads, int tails)
{

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long> dist(0, 1);

    if (dist(mt) == 0)
    {
        return heads;
    }
    else
    {
        return tails;
    }
}
