from interface import Interface
from person import Person
import random
import threading
import time
import stats

interface_lock = threading.Lock()
population_lock = threading.Lock()
alive = 0
interface = Interface()
interface.init_interface()
population = list()
threads = list()


def person_thread(person, person_number):
    global alive
    alive += 1

    while not interface.is_curses_initialized:
        time.sleep(.1)

    while person.alive:
        with interface_lock:
            interface.print_person(person_number, person)

        time.sleep(1)
        person.increase_age()

        if person.age >= person.life_expectancy:
            person.kill()

    with interface_lock:
        interface.print_person_death(person_number, person)

    alive -= 1
    time.sleep(.1)


def add_to_population():
    person = Person()
    person.init_child()
    population.append(person)
    t = threading.Thread(target=person_thread, args=[
                         person, len(population) - 1])
    t.start()
    threads.append(t)


def interface_thread():
    with interface_lock:
        interface.init_interface()
        interface.print_info()

    global alive
    while alive > 0:
        with interface_lock:
            interface.refresh_display()
        time.sleep(1)

    with interface_lock:
        interface.end_curses()

    time.sleep(.1)

def population_thread():
    global alive
    while alive > 0:
        time.sleep(2)
        global population
        currentPopulationSize = len(population) - 1
        with population_lock:
            for i in range(0, currentPopulationSize):
                for j in range(0, currentPopulationSize):
                    if population[i].gender != population[j].gender:
                       if population[i].can_have_child & population[j].can_have_child:
                           if random.randint(0, 100) < stats.BIRTH_RATIO_YEARLY:
                                person = Person()
                                person.init_random_person()
                                population.append(person)
                                t = threading.Thread(target=person_thread, args=[currentPopulationSize + 1, currentPopulationSize + 1])
                                t.start()
                                threads.append(t)
                                break
                if random.randint(0, 100) <= stats.ACCIDENT_RATIO:
                    random_kill = random.randint(0, len(population))
                    population[random_kill].alive = False
                if alive == 1:
                    alive = 0
    time.sleep(1)

def main():

    for i in range(stats.STARTING_POPULATION_SIZE):
        person = Person()
        person.init_random_person()
        population.append(person)
        t = threading.Thread(target=person_thread, args=[population[i], i])
        t.start()
        threads.append(t)

    add_to_population()

    population_t = threading.Thread(target=population_thread)
    population_t.start()
    population_t.join()

    interface_t = threading.Thread(target=interface_thread)
    interface_t.start()
    interface_t.join()

    for thread in threads:
        thread.join()


main()
