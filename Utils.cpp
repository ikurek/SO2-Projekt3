#include <iostream>
#include "Utils.h"

using namespace std;

int Utils::randomInRange(int start, int end) {

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(start, end);

    return dist(mt);
}

int Utils::randomCoinFlip(int heads, int tails) {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    if (dist(mt) == 0) {
        return heads;
    } else {
        return tails;
    }
}




