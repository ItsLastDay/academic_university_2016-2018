CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++14

all: ./bin/main
	./bin/main


./bin/main: ./src/main.cpp ./src/shapes.h
	$(CPP) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm ./bin/*
