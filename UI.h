#include <iostream>
#include <random>
#include <ncurses.h>
#include "Person.h"
#include "Statistics.h"


using namespace std;

class UI
{
  public:
    bool isCursesInitialized = false;
    void initUI();
    void initCurses();
    void initCursesColours();
    void getConsoleSize();
    void printStaticUI();
    void refreshDisplay();
    void printPerson(int, Person);
    void printPersonDeath(int, Person);
    void printInfo();
    void endCurses();

  private:
    unsigned int consoleX;
    unsigned int consoleY;
};
