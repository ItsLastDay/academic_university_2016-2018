CPP = g++
CPPFLAGS2 = -Wall -Werror -Wextra -pedantic -std=c++14 -O0 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector -ggdb

CPPFLAGS = -Wall -Werror -Wextra -std=c++14 -O0

all: ./bin/main
	./bin/main

./bin/main: ./src/main.cpp ./src/graph.h ./src/filtered_graph.h ./src/shortest_path.h
	$(CPP) $(CPPFLAGS) $< -o $@


.PHONY: clean

clean:
	rm ./bin/*
