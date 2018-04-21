#include "Person.h"
#include "Statistics.h"
#include "Utils.h"

using namespace std;

// Initializes a random person for starting population
void Person::initRandom() {
    this->age = Utils::randomInRange(1, Statistics::ageLimit / 2);
    this->children = Utils::randomInRange(0, Statistics::maxChildrenForPerson);
}

// Initializes a child
void Person::initBeginning() {
    this->age = 0;
    this->children = 0;
}

// Initializes common person data
Person::Person() {
    this->alive = true;
    this->gender = Utils::randomCoinFlip(0, 1);
    this->lifeExpectancy = Utils::randomInRange(Statistics::ageLimit / 2, Statistics::ageLimit);
}

void Person::increaseAge() {
    this->age++;
}

void Person::kill() {
    this->alive = false;
}

// Returns true, is person is able to have a child
// Otherwise, returns false
bool Person::canHaveChild() {

    if (this->alive == 0) return false;
    if (this->children >= Statistics::maxChildrenForPerson) return false;
    if (this->age >= Statistics::birthMaximumAge) return false;
    if (this->age <= Statistics::birthMinimumAge) return false;

    return true;
}

