CC=g++
#CXXFLAGS=-std=c++11
CXXFLAGS=-std=c++11 -g -O0
SRC=../interface/interface.cpp pgetnextnode.cpp pselectnode.cpp pjoinnode.cpp demo.cpp pcrossproductnode.cpp
OBJ=../interface/interface.o pgetnextnode.o pselectnode.o pjoinnode.o demo.o pcrossproductnode.o

%.o: %.cpp %.h
	$(CC) $(CXXFLAGS) -c $< -o $@

demo: $(OBJ)
	$(CC) $(OBJ) $(CXXFLAGS) -o $@

valgrind: demo
	valgrind --leak-check=full --track-origins=yes ./demo

clean:
	rm -f demo
	rm -f *.o
	rm -f ../interface/*.o
