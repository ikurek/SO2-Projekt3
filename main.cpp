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

// Synchronizing operations on population
mutex population_mutex;
condition_variable population_cv;

vector<Person> population;
vector<thread> threads;

// Returns a number of alive person objects in population
int calculateAlivePersons()
{
    int alive = 0;

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
    
    while (!ui.isCursesInitialized)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    

    // Represents a lifespan of person
    while (person.alive)
    {

        ui.printPerson(personNumber, person);

        this_thread::sleep_for(chrono::milliseconds(972));

        person.increaseAge();

        // Kill if age is higher then expected
        if (person.age >= person.lifeExpectancy)
        {
            person.kill();
        }

        // Check for specified accident probability
        if (Utils::randomInRange(0, 1000) <= Statistics::accidentRatioInPercent)
        {
            // If matched, find element alive
            // And kill it
            person.kill();
        }
    }

    ui.printPersonDeath(personNumber, person);
    this_thread::sleep_for(chrono::milliseconds(1000));
}

// Locks population_mutex
// Acquires unique lock on population_cv
// Adds person object to population
// Returns index of newely added object
void addToPopulation()
{

    Person person = Person();
    person.initBeginning();

    population.emplace_back(person);
    threads.emplace_back(thread(personThread, ref(population[population.size() - 1]), population.size() - 1));
}

// Thread performs operations on a whole population
// Causes childbirth, accidents, etc.
void populationThread()
{

    // Lasts till there are people alive
    while (calculateAlivePersons() > 0)
    {

        // Sleep thread for 1,5s
        this_thread::sleep_for(chrono::milliseconds(1973));

        // Save population size in current state
        unsigned long currentPopulationSize = population.size() - 1;

        // Iterate over all population elements to cause childbirth
        // FIXME: Matches elements wrong
        unique_lock<mutex> lck(population_mutex);
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
        lck.unlock();
        population_cv.notify_all();
    }

    this_thread::sleep_for(chrono::milliseconds(1000));
}

// Thread handles UI and Log printing
void uiThread()
{
    // Create UI object
    ui.initUI();
    ui.printInfo();

    // Works till there are people alive
    while (calculateAlivePersons() > 0)
    {        
        ui.refreshDisplay();
        // Wait for some time
        this_thread::sleep_for(chrono::milliseconds(200));
        // Go to next year
    }

    ui.endCurses();
    this_thread::sleep_for(chrono::milliseconds(1000));
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
    for (int j = 0; j < population.size(); ++j)
    {
        threads.emplace_back(thread(personThread, ref(population[j]), j));
    }

    // Start thread that causes population events
    thread populationT = thread(populationThread);

    // Start and join thread that controlls UI
    thread uiT = thread(uiThread);
    uiT.join();
    populationT.join();

    finallize();
    return 0;
}
