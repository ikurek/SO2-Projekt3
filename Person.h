#pragma once

#include <thread>
#include <vector>

using namespace std;

class Person
{

  public:
    int age;
    int lifeExpectancy;
    int children;
    bool gender;
    bool alive;

    Person();

    void initRandom();

    void initBeginning();

    void increaseAge();

    void kill();

    bool canHaveChild();

  private:
};
