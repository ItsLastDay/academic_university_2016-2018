CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g
BIN=./bin
SRC=./src

TARGET=test


all: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(BIN)/$(TARGET).o $(BIN)/duration.o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/duration.o: $(SRC)/duration.cpp $(SRC)/duration.h
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN)/$(TARGET).o: $(SRC)/test.cpp $(SRC)/duration.h
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/*
