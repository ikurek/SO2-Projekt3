#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Statistics.h"
#include "Utils.h"
#include "Person.h"


using namespace std;

mutex logger_mutex;
condition_variable logger_cv;

mutex population_mutex;
condition_variable population_cv;

vector<Person> population;
vector<thread> threads;
vector<string> logger;

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


void removeFromPopulation(unsigned long id) {

}

// Thread represents a single person inside a population
void personThread(Person &person, int personNumber) {

    // If person age is equal to 0, log
    // That a new person was born
    if (person.age == 0) {
        addToLog("(+) " + person.name + " was born");
    }


    while (person.alive) {
        this_thread::sleep_for(chrono::milliseconds(500));
        person.increaseAge();

        if (person.age >= Statistics::ageLimit) {
            person.alive = false;
        }

    }
    addToLog((("(-) " + person.name) + " died at the age of ") + to_string(person.age));
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

void populationThread() {

    while (!threads.empty()) {

        this_thread::sleep_for(chrono::milliseconds(1500));

        addToPopulation();

        for (int i = 0; i < population.size() - 1; ++i) {
            for (int j = 0; j < population.size() - 1; ++j) {
                if (population[i].alive && population[j].alive) {

                }
            }
        }
    }
}

void uiThread() {

    int year = 0;

    while (!threads.empty()) {

        this_thread::sleep_for(chrono::milliseconds(2000));

        cout << "Year " << year << ", " << threads.size() << " still alive" << endl;
        printLog();

        year++;
    }

    cout << "Population died in " << year << " years" << endl;
}

void printPopulation(vector<Person> population) {

    for (unsigned int i = 0; i < population.size(); ++i) {
        cout << i << ". Name: " << population[i].name << " Age: " << population[i].age << endl;
    }
}

void finallize() {
    population.clear();
    threads.clear();
}

int main() {

    for (int i = 0; i < 20; ++i) {
        Person person = Person();
        person.initRandom();
        population.emplace_back(person);
    }


    printPopulation(population);

    for (int j = 0; j < population.size(); j++) {
        threads.emplace_back(thread(personThread, ref(population[j]), j));
    }

    thread populationT = thread(populationThread);

    thread uiT = thread(uiThread);
    uiT.join();

    finallize();


    return 0;
}


