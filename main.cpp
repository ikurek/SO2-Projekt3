#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Statistics.h"
#include "Utils.h"
#include "Person.h"


using namespace std;

// Synchronizing log output
mutex logger_mutex;
condition_variable logger_cv;

// Synchronizing operations on population
mutex population_mutex;
condition_variable population_cv;

vector<Person> population;
vector<thread> threads;
vector<string> logger;

// Returns a number of alive person objects in population
unsigned long calculateAlivePersons() {
    unsigned long alive = 0;

    for (int i = 0; i < population.size() - 1; ++i) {
        if (population[i].alive) alive++;
    }

    return alive;
}

// Locks logger_mutex
// Acquires unique lock on logger_cv
// Adds element to logger
void addToLog(string text) {
    unique_lock<mutex> lck(logger_mutex);
    logger_cv.wait(lck);
    logger.emplace_back(text);
    lck.unlock();
    logger_cv.notify_one();
}

// Locks logger_mutex
// Acquires unique lock on logger_cv
// Prints content of log
void printLog() {

    lock_guard<mutex> lck(logger_mutex);

    if (logger.empty()) {
        cout << "   ==> Nothing interesting happened" << endl;
    } else {
        for (const auto &i : logger) {
            cout << "   ==> " << i << endl;
        }
    }
    logger.clear();
    logger_cv.notify_one();
}

// Thread represents a single person inside a population
void personThread(Person &person, int personNumber) {

    // If person age is equal to 0, log
    // That a new person was born
    if (person.age == 0) {
        addToLog("(+) " + to_string(personNumber) + " | Gender: " + to_string(person.gender) + " | Exp. age: " +
                 to_string(person.lifeExpectancy));
    }

    // Represents a lifespan of person
    while (person.alive) {
        this_thread::sleep_for(chrono::milliseconds(500));
        person.increaseAge();

        // Kill if age is higher then expected
        if (person.age >= person.lifeExpectancy) {
            person.kill();
        }

    }

    // If age was equal or more than expected, cause of death was natural
    // In other case, person died in an accident
    if (person.age >= person.lifeExpectancy) {
        addToLog((("(-) " + to_string(personNumber)) + " | Age: ") + to_string(person.age) +
                 " | Cause: Natural | Children: " + to_string(person.children));
    } else {
        addToLog((("(-) " + to_string(personNumber)) + " | Age: ") + to_string(person.age) +
                 " | Cause: Accident | Children: " + to_string(person.children));
    }


}

// Locks population_mutex
// Acquires unique lock on population_cv
// Adds person object to population
// Returns index of newely added object
void addToPopulation() {

    Person person = Person();
    person.initBeginning();

    lock_guard<mutex> lck(population_mutex);
    population.emplace_back(person);
    threads.emplace_back(thread(personThread, ref(population[population.size() - 1]), population.size() - 1));

    population_cv.notify_all();
}

// Thread performs operations on a whole population
// Causes childbirth, accidents, etc.
void populationThread() {

    // Lasts till there are people alive
    while (calculateAlivePersons() > 0) {

        // Sleep thread for 1,5s
        this_thread::sleep_for(chrono::milliseconds(1500));

        // Save population size in current state
        unsigned long currentPopulationSize = population.size() - 1;

        // Iterate over all population elements to cause childbirth
        // FIXME: Everyone fuck everybody, what the hell
        for (int i = 0; i < currentPopulationSize; ++i) {
            for (int j = 0; j < currentPopulationSize; ++j) {
                // Check if given people have different gendes
                if (population[i].gender != population[j].gender) {
                    // Check if they can have children
                    if (population[i].canHaveChild() && population[j].canHaveChild()) {
                        // Check with specified distribution
                        if (Utils::randomInRange(0, 100) < Statistics::birthRatioInYearPercent) {
                            // Create new population element (child)
                            // Ich checks succeeded
                            addToPopulation();
                            break;
                        }
                    }
                }
            }
        }

        // Check for specified accident probability
        if (Utils::randomInRange(0, 100) <= Statistics::accidentRatioInPercent) {
            // If matched, find element alive
            // And FUCKING KILL IT
            int index = Utils::randomInRange(0, population.size() - 1);
            while (!population[index].alive) {
                index = Utils::randomInRange(0, population.size() - 1);
            }
            population[index].kill();
        }

    }
}

// Thread handles UI and Log printing
void uiThread() {

    // Begin counting from one year
    int year = 1;

    // Works till there are people alive
    while (calculateAlivePersons() > 0) {
        // Print current year with stats
        cout << "Year " << year << " passed, " << calculateAlivePersons() << " still alive" << endl;
        // Wait for some time
        this_thread::sleep_for(chrono::milliseconds(2000));
        // Dump logs to terminal
        printLog();
        // Go to next year
        year++;
    }

    // Info when everybody dies
    cout << "Population died in " << year << " years, had " << population.size() << " people total" << endl;
}

// Clear some stuff before finishing
void finallize() {
    population.clear();
    threads.clear();
}

int main() {

    // Create starting population
    for (int i = 0; i < Statistics::startingPopulationSize; ++i) {
        Person person = Person();
        person.initRandom();
        population.emplace_back(person);
    }

    // Start thread for each person in population
    for (int j = 0; j < population.size(); j++) {
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


