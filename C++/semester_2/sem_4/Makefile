CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++11

all: ./bin/main
	./bin/main


./bin/main: ./src/class_tests.cpp ./src/event.h
	$(CPP) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm ./bin/*
