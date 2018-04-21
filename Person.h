#ifndef SO2_PROJEKT3_PERSONTHREAD_H
#define SO2_PROJEKT3_PERSONTHREAD_H

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

class Person {

public:
    string name;
    int age;
    int gender;
    int alive;

    Person();

    void initRandom();

    void initBeginning();

    void increaseAge();

private:
    vector<string> femaleNames = {"Anna", "Maria", "Katarzyna", "Małgorzata", "Agnieszka", "Krystyna", "Barbara", "Ewa",
                                  "Elżbieta", "Zofia", "Janina", "Teresa", "Joanna", "Magdalena", "Monika", "Jadwiga",
                                  "Danuta", "Irena", "Halina", "Helena", "Beata", "Aleksandra", "Marta", "Dorota",
                                  "Marianna", "Grażyna", "Jolanta", "Stanisława", "Iwona", "Karolina", "Bożena",
                                  "Urszula", "Justyna", "Renata", "Alicja", "Paulina", "Sylwia", "Natalia", "Wanda",
                                  "Agata", "Aneta", "Izabela", "Ewelina", "Marzena", "Wiesława", "Genowefa", "Patrycja",
                                  "Kazimiera", "Edyta", "Stefania"};
    vector<string> maleNames = {"Jan", "Andrzej", "Piotr", "Krzysztof", "Stanisław", "Tomasz", "Paweł", "Józef",
                                "Marcin", "Marek", "Michał", "Grzegorz", "Jerzy", "Tadeusz", "Adam", "Łukasz",
                                "Zbigniew", "Ryszard", "Dariusz", "Henryk", "Mariusz", "Kazimierz", "Wojciech",
                                "Robert", "Mateusz", "Marian", "Rafał", "Jacek", "Janusz", "Mirosław", "Maciej",
                                "Sławomir", "Jarosław", "Kamil", "Wiesław", "Roman", "Władysław", "Jakub", "Artur",
                                "Zdzisław", "Edward", "Mieczysław", "Damian", "Dawid", "Przemysław", "Sebastian",
                                "Czesław", "Leszek", "Daniel", "Waldemar"};

    string getRandomName();
};


#endif //SO2_PROJEKT3_PERSONTHREAD_H
