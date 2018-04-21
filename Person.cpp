#include "Person.h"
#include "Statistics.h"
#include "Utils.h"

using namespace std;

void Person::initRandom() {
    this->age = Utils::randomInRange(1, Statistics::ageLimit / 2);
}

void Person::initBeginning() {
    this->age = 0;
}

Person::Person() {
    this->alive = true;
    this->gender = Utils::randomCoinFlip(0, 1);
    this->name = "TEST";
}

string Person::getRandomName() {

    if (this->gender == 0) {
        return maleNames[Utils::randomInRange(0, maleNames.size() - 1)];
    } else {
        return femaleNames[Utils::randomInRange(0, femaleNames.size() - 1)];
    }
}

void Person::increaseAge() {
    this->age++;

}
