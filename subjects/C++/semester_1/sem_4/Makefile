CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g
BIN=./bin
SRC=./src

TARGET=main


all: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(BIN)/$(TARGET).o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/$(TARGET).o: $(SRC)/$(TARGET).cpp
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/*
