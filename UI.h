#include <iostream>
#include <random>
#include <ncurses.h>

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
    void printPerson(int, int, int, bool, unsigned long, unsigned long);
    void printPersonDeath(int, int, int, bool, unsigned long, unsigned long);
    void printYear(int);
    void endCurses();

  private:
    mutex mtx;
    unsigned int consoleX;
    unsigned int consoleY;
};
