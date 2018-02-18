CPP = g++
CPPFLAGS = -O0 -Wall -Werror -std=c++14 -pedantic -Wextra

all: ./bin/main
	valgrind ./bin/main


./bin/main: ./src/main.cpp ./src/au_allocator.h
	$(CPP) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm ./bin/*
