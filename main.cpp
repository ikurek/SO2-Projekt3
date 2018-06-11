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

// Mutex to synchronize UI operations
mutex ui_mutex;

// Synchronizing operations on population
mutex population_mutex;
condition_variable population_cv;

// Vectors to store threads and person objects
vector<Person> population;
vector<thread> threads;

// Holding ammount of people alive
// Starting is 1, chence 0 stops program
int alive = 1;

// Thread represents a single person inside a population
void personThread(Person &person, int personNumber)
{
    // Increase counter for alive people
    alive++;

    // Wait until NCurses library is initialized
    while (!ui.isCursesInitialized)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Iterate as long, as person is alive
    while (person.alive)
    {
        // Lock mutex for UI
        ui_mutex.lock();

        // Print current state of person
        ui.printPerson(personNumber, person);

        // Free UI
        ui_mutex.unlock();

        // Sleep for one second
        this_thread::sleep_for(chrono::milliseconds(1000));

        // Increase age of person in this thread
        person.increaseAge();

        // Kill if age is higher then expected
        if (person.age >= person.lifeExpectancy)
        {
            person.kill();
        }
    }

    // Lock UI mutex
    ui_mutex.lock();

    // Print death sentence for person
    ui.printPersonDeath(personNumber, person);

    // Free UI
    ui_mutex.unlock();

    // Decrease number of people alive
    alive--;

    // Wait for a while before stopping thread
    this_thread::sleep_for(chrono::milliseconds(100));
}


// Method creates a new person object
// With initial data set to child
// Needs to be called from critical section
// Inside mutex/cv lock
void addToPopulation()
{
    // Create new person with initial data
    Person person = Person();
    person.initBeginning();
    // Add person to vector
    population.emplace_back(person);
    // Start thread for newly created person
    threads.emplace_back(thread(personThread, ref(population[population.size() - 1]), population.size() - 1));
}

// Thread performs operations on a whole population
// Causes childbirth, accidents, etc.
void populationThread()
{

    // Lasts till there are people alive
    while (alive > 0)
    {
        // Sleep thread for 2s
        this_thread::sleep_for(chrono::milliseconds(2000));

        // Save population size in current state
        unsigned long currentPopulationSize = population.size() - 1;

        // Acquire unique lock on population mutex
        unique_lock<mutex> lck(population_mutex);

        // Iterate over all population elements to cause childbirth
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
            int random = Utils::randomInRange(0, population.size() - 1);
            population[random].kill();
        }

        // Release lock
        lck.unlock();
        // Notify waiting
        population_cv.notify_all();

        // If thread caused last person to die
        // Mark population as dead
        if (alive == 1)
        {
            alive = 0;
        }
    }

    this_thread::sleep_for(chrono::milliseconds(100));
}

// Thread handles UI and Log printing
void uiThread()
{
    // Acquire lock on UI
    ui_mutex.lock();

    // Initialize NCurses
    ui.initUI();

    // Print information about population
    ui.printInfo();

    // Release UI
    ui_mutex.unlock();

    // Works till there are people alive
    while (alive > 0)
    {
        // Acquire UI lock
        ui_mutex.lock();
        
        // Refresh NCurses
        ui.refreshDisplay();

        // Release lock
        ui_mutex.unlock();

        // Sleep for one second
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // Lock UI
    ui_mutex.lock();

    // Finalize Curses
    ui.endCurses();

    // Unlock UI
    ui_mutex.unlock();

    // Wait for some time before finishing thread
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Clear some stuff before finishing
void finallize()
{

    // Wait for threads to finish execution
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Clear vectors
    population.clear();
    threads.clear();
}

// Entry point
int main()
{
    // Create starting population
    for (int i = 0; i < Statistics::startingPopulationSize; ++i)
    {
        // Create new person
        Person person = Person();

        // Initialize person with random data
        person.initRandom();

        // Place person inside population
        population.emplace_back(person);
    }

    // Start thread for each person in population
    for (int j = 0; j < population.size(); ++j)
    {
        // Start thread and place it into vector
        threads.emplace_back(personThread, ref(population[j]), j);
    }

    // Start thread that causes population events
    thread populationT = thread(populationThread);

    // Start and join thread that controlls UI
    thread uiT = thread(uiThread);
    
    // Join threads
    populationT.join();
    uiT.join();

    finallize();
    return 0;
}
