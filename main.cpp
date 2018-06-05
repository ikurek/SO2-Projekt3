#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include "Statistics.h"
#include "Utils.h"
#include "Person.h"
#include "UI.h"

using namespace std;

// Single instance of UI object
UI ui;

// Synchronizing log output
condition_variable logger_cv;

// Synchronizing operations on population
mutex population_mutex;
condition_variable population_cv;

vector<Person> population;
vector<thread> threads;

// Returns a number of alive person objects in population
unsigned long calculateAlivePersons()
{
    unsigned long alive = 0;

    for (int i = 0; i < population.size() - 1; ++i)
    {
        if (population[i].alive)
            alive++;
    }

    return alive;
}

// Thread represents a single person inside a population
void personThread(Person &person, int personNumber)
{

    // Get random colour from palette
    int color = (personNumber % 6) + 1;
    // If person age is equal to 0, log
    // That a new person was born
    if (person.age == 0)
    {
    }

    while (!ui.isCursesInitialized)
    {
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    // Represents a lifespan of person
    while (person.alive)
    {

        ui.printPerson(personNumber, person.age, Statistics::ageLimit, person.gender, person.children, person.lifeExpectancy);

        this_thread::sleep_for(chrono::milliseconds(500));
        person.increaseAge();

        // Kill if age is higher then expected
        if (person.age >= person.lifeExpectancy)
        {
            person.kill();
        }
    }

    ui.printPersonDeath(personNumber, person.age, Statistics::ageLimit, person.gender, person.children, person.lifeExpectancy);
}

// Locks population_mutex
// Acquires unique lock on population_cv
// Adds person object to population
// Returns index of newely added object
void addToPopulation()
{

    Person person = Person();
    person.initBeginning();

    lock_guard<mutex> lck(population_mutex);
    population.emplace_back(person);
    threads.emplace_back(thread(personThread, ref(population[population.size() - 1]), population.size() - 1));

    population_cv.notify_all();
}

// Thread performs operations on a whole population
// Causes childbirth, accidents, etc.
void populationThread()
{

    // Lasts till there are people alive
    while (calculateAlivePersons() > 0)
    {

        // Sleep thread for 1,5s
        this_thread::sleep_for(chrono::milliseconds(1500));

        // Save population size in current state
        unsigned long currentPopulationSize = population.size() - 1;

        // Iterate over all population elements to cause childbirth
        // FIXME: Matches elements wrong
        for (int i = 0; i < currentPopulationSize; ++i)
        {
            for (int j = 0; j < currentPopulationSize; ++j)
            {
                // Check if given people have different gendes
                if (population[i].gender != population[j].gender)
                {
                    // Check if they can have children
                    if (population[i].canHaveChild() && population[j].canHaveChild())
                    {
                        // Check with specified distribution
                        if (Utils::randomInRange(0, 100) < Statistics::birthRatioInYearPercent)
                        {
                            // Create new population element (child)
                            // If checks succeeded
                            population[i].children++;
                            population[j].children++;
                            addToPopulation();
                            break;
                        }
                    }
                }
            }
        }

        // Check for specified accident probability
        if (Utils::randomInRange(0, 100) <= Statistics::accidentRatioInPercent)
        {
            // If matched, find element alive
            // And kill it
            int index = Utils::randomInRange(0, population.size() - 1);
            while (!population[index].alive)
            {
                index = Utils::randomInRange(0, population.size() - 1);
            }
            population[index].kill();
        }
    }
}

// Thread handles UI and Log printing
void uiThread()
{
    // Create UI object
    ui.initUI();

    // Works till there are people alive
    while (calculateAlivePersons() > 0)
    {
        ui.refreshDisplay();
        // Wait for some time
        this_thread::sleep_for(chrono::milliseconds(20));
        // Go to next year
    }

    ui.endCurses();
}

// Clear some stuff before finishing
void finallize()
{
    population.clear();
    threads.clear();
}

int main()
{

    // Create starting population
    for (int i = 0; i < Statistics::startingPopulationSize; ++i)
    {
        Person person = Person();
        person.initRandom();
        population.emplace_back(person);
    }

    // Start thread for each person in population
    for (int j = 0; j < population.size(); j++)
    {
        threads.emplace_back(thread(personThread, ref(population[j]), j));
    }

    // Start thread that causes population events
    thread populationT = thread(populationThread);

    // Start and join thread that controlls UI
    thread uiT = thread(uiThread);
    uiT.join();

    finallize();
    return 0;
}
