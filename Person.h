#include <thread>
#include <vector>

using namespace std;

class Person
{

  public:
    unsigned long age;
    unsigned long lifeExpectancy;
    unsigned long children;
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
