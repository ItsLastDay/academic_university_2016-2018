CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++11

all: ./bin/main

./bin/my_algorithms.obj: ./src/my_algorithms.cpp ./src/my_algorithms.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/main.obj: ./src/main.cpp ./src/my_algorithms.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/main: ./bin/main.obj ./bin/my_algorithms.obj
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm ./bin/*
