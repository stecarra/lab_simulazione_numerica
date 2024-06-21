CC = g++
CFLAGS = -Wall -O3 --std=c++11

main.exe : main.o city.o trip.o population.o random.o
	$(CC) main.o city.o trip.o population.o random.o -o main.exe

main.o : main.cpp trip.h population.h city.h random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

city.o : city.cpp city.h random.h
	$(CC) -c city.cpp -o city.o $(CFLAGS)

trip.o : trip.cpp trip.h city.h random.h
	$(CC) -c trip.cpp -o trip.o $(CFLAGS)

population.o : population.cpp population.h trip.h city.h random.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o main.exe
