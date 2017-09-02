CC=g++
#CXXFLAGS=-std=c++11
CXXFLAGS=-std=c++11 -g -O0
SRC=../interface/interface.cpp pgetnextnode.cpp pselectnode.cpp pjoinnode.cpp demo.cpp pcrossproductnode.cpp pmergejoin.cpp phashjoin.cpp histogram.cpp
OBJ=../interface/interface.o pgetnextnode.o pselectnode.o pjoinnode.o demo.o pcrossproductnode.o pmergejoin.o phashjoin.o histogram.o

valgrind: demo
	valgrind --leak-check=full --track-origins=yes ./demo

demo: $(OBJ)
	$(CC) $(OBJ) $(CXXFLAGS) -o $@

%.o: %.cpp %.h
	$(CC) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f demo
	rm -f *.o
	rm -f ../interface/*.o
