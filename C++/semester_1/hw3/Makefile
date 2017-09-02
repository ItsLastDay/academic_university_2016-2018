CPP = g++
CPPFLAGS = -Wall -Werror -std=c++11 -O0 -g -DMY_COMPUTER

all: ./bin/result
	./bin/result
# valgrind --leak-check=full 

debug: ./bin/result
	gdb ./bin/result
	
./bin/any.o: ./src/any.cpp ./src/any.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/main.o: ./src/test2.cpp ./src/any.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/result: ./bin/any.o ./bin/main.o
	$(CPP) $(CPPFLAGS) $^ -o ./bin/result


clean:
	rm -rf ./bin

.PHONY: clean all debug
