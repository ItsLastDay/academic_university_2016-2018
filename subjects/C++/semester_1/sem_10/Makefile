CPP = g++
CPPFLAGS = -std=c++11 -Wall -Werror

all: ./bin/result

./bin/main.o: ./src/main.cpp ./src/standard_funcs.h ./src/fifo.h ./src/priorty_queue.h
	$(CPP) $(CPPFLAGS) -c $< -o $@ 


./bin/result: ./bin/main.o 
	$(CPP) $(CPPFLAGS) $^ -o $@ 

clean:
	rm -r ./bin/*

.PHONY: clean all
