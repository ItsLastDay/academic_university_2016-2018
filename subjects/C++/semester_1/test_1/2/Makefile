CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g
BIN=./bin
SRC=./src

TARGET=mergesort


all: $(BIN)/$(TARGET)

valgrind: $(BIN)/$(TARGET)
	valgrind --leak-check=full --track-origins=yes $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(BIN)/$(TARGET).o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/$(TARGET).o: $(SRC)/main.cpp $(SRC)/mergesort.h
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/*
