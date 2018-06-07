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

void UI::printPerson(int id, Person person)
{
        mtx.lock();
        attron(COLOR_PAIR(2));
        mvprintw(id + 2, 0, "[");
        if (person.children > 3)
                person.children = 3;
        if (person.age > person.lifeExpectancy)
                person.age = person.lifeExpectancy;

        if (person.gender)
        {
                mvprintw(id + 2, Statistics::ageLimit, "] Gender: Male,    Children: %d,  Age: %3d", person.children, person.age);
        }
        else
        {
                mvprintw(id + 2, Statistics::ageLimit, "] Gender: Female,  Children: %d,  Age: %3d", person.children, person.age);
        }
        mvprintw(id + 2, person.age - 1, " ");
        mvprintw(id + 2, person.age, "#");
        mtx.unlock();
}

void UI::printPersonDeath(int id, Person person)
{
        mtx.lock();
        string deathCause;

        if (person.lifeExpectancy >= person.age)
        {
                deathCause = "natural,  ";
                attron(COLOR_PAIR(1));
                
        }
        else
        {
                deathCause = "accident, ";
                attron(COLOR_PAIR(3));
        }
        
        mvprintw(id + 2, 0, "[");

        if (person.gender)
        {
                mvprintw(id + 2, Statistics::ageLimit, "] Death: %s", deathCause.c_str());
        }
        else
        {
                mvprintw(id + 2, Statistics::ageLimit, "] Death: %s", deathCause.c_str());
        }
        mvprintw(id + 2, person.age - 1, " ");
        mvprintw(id + 2, person.age, "#");
        mtx.unlock();
}

void UI::printInfo()
{
        mtx.lock();
        attron(COLOR_PAIR(4) | A_BOLD);
        mvprintw(0, 0, "Starting population: %d || Birth ratio: %d%% || Birth age: %d - %d || Accident ratio: %d%% || Max children: %d", Statistics::startingPopulationSize, Statistics::birthRatioInYearPercent, Statistics::birthMinimumAge, Statistics::birthMaximumAge, Statistics::accidentRatioInPercent, Statistics::maxChildrenForPerson);
        mtx.unlock();
}

void UI::endCurses()
{
        endwin();
}
