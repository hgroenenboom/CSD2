
paint: paint.o figuur.o vierkant.o
	g++ -o paint paint.o figuur.o vierkant.o

paint.o: paint.cpp
	g++ -c paint.cpp

figuur.o: figuur.cpp figuur.h
	g++ -c figuur.cpp

vierkant.o: vierkant.cpp vierkant.h
	g++ -c vierkant.cpp

clean:
	rm -f *.o paint

