CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g

all: result

shared_buffer.o: shared_buffer.c shared_buffer.h
	$(CPP) $(CFLAGS) -c $< -o $@

lazy_string.o: lazy_string.c lazy_string.h
	$(CPP) $(CFLAGS) -c $< -o $@

result.o: main.cpp shared_buffer.h
	$(CPP) $(CFLAGS) -c $< -o $@

result: result.o shared_buffer.o lazy_string.o
	$(CPP) $(CFLAGS) $^ -o $@

.PHONY: clean all

clean:
	rm *.o
