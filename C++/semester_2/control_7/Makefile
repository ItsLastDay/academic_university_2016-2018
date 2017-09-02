BIN_DIR=./bin/
SRC_DIR=./src/

SAFE_DL_H=$(SRC_DIR)/safe_dl.h
TEST=$(BIN_DIR)/test
SHARED=$(BIN_DIR)/shared.so
SHARED2=$(BIN_DIR)/shared2.so
CPP_FLAGS=-std=c++14 -Wall  -fPIC -ggdb -D_GNU_SOURCE --pedantic -Wextra

# Comment this and uncomment over definition to turn off tests
TASK2=-DTASK2
#TASK3=-DTASK3

#TASK2=
TASK3=

all: $(TEST) $(SHARED) $(SHARED2)

$(TEST): $(SRC_DIR)/test.cpp $(SAFE_DL_H) $(SRC_DIR)/catch.hpp  
	g++-5  $< $(CPP_FLAGS) $(TASK2) $(TASK3) -o $@ -ldl

$(SHARED): $(SRC_DIR)/shared_lib.cpp $(SAFE_DL_H)
	g++-5 -shared $< $(CPP_FLAGS) $(TASK2) $(TASK3) -o $@ -ldl

$(SHARED2): $(SRC_DIR)/shared_lib.cpp $(SAFE_DL_H)
	g++-5 -shared $< $(CPP_FLAGS) $(TASK2) $(TASK3) -DSHARED2 -o $@ -ldl

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: clean all
