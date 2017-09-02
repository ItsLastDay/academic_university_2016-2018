BIN_DIR=./bin/
SRC_DIR=./src/
LIB_NAME=libmypthread
LIB_OBJ=$(BIN_DIR)/$(LIB_NAME).o
LIB_SHARED=$(BIN_DIR)/$(LIB_NAME).so
LIB_STATIC=$(BIN_DIR)/$(LIB_NAME).a
THREAD_LIBS=$(LIB_SHARED) $(LIB_STATIC)
TESTS=$(BIN_DIR)/test_bench $(BIN_DIR)/test_recursive 

CPP_FLAGS=-std=c++14 -Wall -Werror -fPIC -ggdb
LINK_FLAGS=
LIBS=-ldl

all: $(TESTS)

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $<

$(LIB_SHARED): $(LIB_OBJ)
	g++ -shared $< $(LINK_FLAGS) -o $@ $(LIBS)

$(LIB_OBJ): $(SRC_DIR)/libmypthread.cpp
	g++ -c $<  $(CPP_FLAGS) -o $@

$(BIN_DIR)/test_bench: $(SRC_DIR)/test_bench.cpp $(THREAD_LIBS)
	g++ $< $(CPP_FLAGS) -pthread -o $@ $(LIBS)

$(BIN_DIR)/test_recursive: $(SRC_DIR)/test_recursive.cpp $(THREAD_LIBS)
	g++ $< $(CPP_FLAGS) -pthread -o $@ $(LIBS)

clean:
	rm -rf $(BIN_DIR)/*

run_test_bench: $(TESTS)
	LD_PRELOAD=$(LIB_SHARED) $(BIN_DIR)/test_bench 4

run_test_recursive: $(TESTS)
	LD_PRELOAD=$(LIB_SHARED) $(BIN_DIR)/test_recursive 

.PHONY: clean all run_test_bench run_test_recursive
