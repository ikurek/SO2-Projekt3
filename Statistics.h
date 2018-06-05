#ifndef SO2_PROJEKT3_STATISTICS_H
#define SO2_PROJEKT3_STATISTICS_H

#include <chrono>

using namespace std;

class Statistics
{

  public:
    // Size of population to begin with
    static const unsigned int startingPopulationSize = 15;
    // Maximum age of a person (years)
    static const unsigned int ageLimit = 100;
    // Rate of birth for each year (percent)
    static const unsigned int birthRatioInYearPercent = 5;
    // Maximum age to have a child (years)
    static const unsigned int birthMaximumAge = 50;
    // Minimum age to have a child (years)
    static const unsigned int birthMinimumAge = 15;
    // Chance of an accident happening to person (percent)
    static const unsigned int accidentRatioInPercent = 10;
    // Maximum number of children a person can have
    static const unsigned int maxChildrenForPerson = 3;
};

#endif //SO2_PROJEKT3_STATISTICS_H
