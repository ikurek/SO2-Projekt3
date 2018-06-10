FILES = main.cpp Person.cpp Statistics.cpp Utils.cpp UI.cpp

all:
	g++ -g -std=c++11 -o main $(FILES) -pthread -lncurses

clean:
	-rm -f *.o
	-rm -f main
	-rm -f *.gch
	-rm -rf *.dSYM

run:
	./main
	clear
