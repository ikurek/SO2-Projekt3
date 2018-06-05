#include "UI.h"

using namespace std;

void UI::initUI()
{
        this->initCurses();
        this->initCursesColours();
        this->getConsoleSize();
        this->isCursesInitialized = true;
}

void UI::initCurses()
{
        initscr();
        scrollok(stdscr, TRUE);
        start_color();
        curs_set(0);
}

void UI::initCursesColours()
{
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
        init_pair(8, COLOR_BLACK, COLOR_WHITE);
}

void UI::getConsoleSize()
{
        unsigned int x, y;
        getmaxyx(stdscr, x, y);

        this->consoleX = x;
        this->consoleY = y;
}

// Repaints content of terminal
void UI::refreshDisplay()
{
        // Lock mutex and refresh curses
        mtx.lock();
        refresh();
        mtx.unlock();
}

void UI::printPerson(int id, int age, int maxAge, bool gender, unsigned long children, unsigned long lifeExpectancy)
{
        mtx.lock();
        attron(COLOR_PAIR(2));
        mvprintw(id + 2, 0, "[");
        if (children > 3)
                children = 3;
        if (age > lifeExpectancy)
                age = lifeExpectancy;

        if (gender)
        {
                mvprintw(id + 2, maxAge, "] Gender: Male,   Children: %d, Age: %3d", children, age);
        }
        else
        {
                mvprintw(id + 2, maxAge, "] Gender: Female, Children: %d, Age: %3d", children, age);
        }
        mvprintw(id + 2, age - 1, " ");
        mvprintw(id + 2, age, "#");
        mtx.unlock();
}

void UI::printPersonDeath(int id, int age, int maxAge, bool gender, unsigned long children, unsigned long lifeExpectancy)
{
        mtx.lock();
        string deathCause;

        if (lifeExpectancy == age)
        {
                deathCause = "natural";
                attron(COLOR_PAIR(1));
                
        }
        else
        {
                deathCause = "accident";
                attron(COLOR_PAIR(3));
        }

        if (children > 3)
                children = 3;
        if (age > lifeExpectancy)
                age = lifeExpectancy;
        
        mvprintw(id + 2, 0, "[");

        if (gender)
        {
                mvprintw(id + 2, maxAge, "] Gender: Male,   Children: %d, Age: %3d, Death: %s", children, age, deathCause.c_str());
        }
        else
        {
                mvprintw(id + 2, maxAge, "] Gender: Female, Children: %d, Age: %3d, Death: %s", children, age, deathCause.c_str());
        }
        mvprintw(id + 2, age - 1, " ");
        mvprintw(id + 2, age, "#");
        mtx.unlock();
}

void UI::printYear(int year)
{
        mtx.lock();
        mvprintw(0, 0, "Year: %d");
        mtx.unlock();
}

void UI::endCurses()
{
        getch();
        endwin();
}
