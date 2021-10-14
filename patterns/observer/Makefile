all: main.o concumer.o slave.o
	g++ main.o concumer.o slave.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

concumer.o: concumer.cpp
	g++ -c concumer.cpp -o concumer.o

slave.o: slave.cpp
	g++ -c slave.cpp -o slave.o

clear:
	rm ./a.out *.o
