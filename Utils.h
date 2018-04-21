#ifndef SO2_PROJEKT3_UTLIS_H
#define SO2_PROJEKT3_UTLIS_H

#include <iostream>
#include <random>

using namespace std;

class Utils {
public:
    static int randomInRange(int start, int end);

    static int randomCoinFlip(int heads, int tails);
};


#endif //SO2_PROJEKT3_UTLIS_H
