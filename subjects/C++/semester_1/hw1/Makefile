CPP=g++
CFLAGS=-Wall -Werror -std=c++11 -g -O2
BIN=./bin
SRC=./src

TARGET=huffman


all: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(BIN)/$(TARGET).o $(BIN)/tree.o
	$(CPP) $(CFLAGS) $^ -o $@ 

$(BIN)/$(TARGET).o: $(SRC)/$(TARGET).cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(BIN)/tree.o: $(SRC)/tree.cpp $(SRC)/tree.h
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: clean all

clean:
	rm -rf $(BIN)/* ./test/*.txt
