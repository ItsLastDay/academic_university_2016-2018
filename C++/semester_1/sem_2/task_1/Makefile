CPP=g++
CFLAGS=-Wall -Werror
BIN=./bin
SRC=./src

all: $(BIN)/main

$(BIN)/main: $(BIN)/add.o $(BIN)/main.o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/main.o: $(SRC)/main.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN)/add.o: $(SRC)/add.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/*
