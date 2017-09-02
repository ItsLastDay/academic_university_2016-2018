CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++14

all: ./bin/main
	./bin/main


./bin/main: ./src/sfinae_test.cpp ./src/serialization.h
	$(CPP) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm ./bin/*
