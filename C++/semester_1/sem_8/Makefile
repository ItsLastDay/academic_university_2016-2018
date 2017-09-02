CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g

all: ./bin/result

./bin/shared_buffer.o: ./src/shared_buffer.c ./src/shared_buffer.h
	$(CPP) $(CFLAGS) -c $< -o $@

./bin/lazy_string.o: ./src/lazy_string.cpp ./src/lazy_string.h
	$(CPP) $(CFLAGS) -c $< -o $@

./bin/result.o: ./src/main.cpp ./src/lazy_string.h
	$(CPP) $(CFLAGS) -c $< -o $@

./bin/result: ./bin/result.o ./bin/shared_buffer.o ./bin/lazy_string.o
	$(CPP) $(CFLAGS) $^ -o $@

.PHONY: clean all

clean:
	rm ./bin/*
