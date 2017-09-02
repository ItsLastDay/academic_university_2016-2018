CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++11

all: ./bin/main


./bin/main: ./src/main.cpp ./src/letter_it.h
	$(CPP) $(CPPFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm ./bin/*
