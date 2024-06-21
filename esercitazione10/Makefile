CC = mpic++
CFLAGS = -Wall -O3 --std=c++11
MPIFLAGS = 

main.exe : main.o city.o trip.o population.o random.o
	$(CC) main.o city.o trip.o population.o random.o -o main.exe $(MPIFLAGS)

main.o : main.cpp trip.h population.h city.h random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS) $(MPIFLAGS)

city.o : city.cpp city.h random.h
	$(CC) -c city.cpp -o city.o $(CFLAGS) $(MPIFLAGS)

trip.o : trip.cpp trip.h city.h random.h
	$(CC) -c trip.cpp -o trip.o $(CFLAGS) $(MPIFLAGS)

population.o : population.cpp population.h trip.h city.h random.h
	$(CC) -c population.cpp -o population.o $(CFLAGS) $(MPIFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS) $(MPIFLAGS)

clean :
	rm -f *.o main.exe

run :
	mpirun -np 4 ./main.exe
