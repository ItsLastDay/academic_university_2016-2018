CPP=g++
CC=gcc
CFLAGS=-Wall -Werror
BIN=./bin
SRC=./src

all: $(BIN)/main

$(BIN)/main: $(BIN)/sum.o $(BIN)/main.o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/sum.o: $(SRC)/sum.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BIN)/main.o: $(SRC)/main.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/*
