bin/test: src/main.cpp src/faults.cpp \
	src/bin_search_tree.h
	g++ -Wall -Werror -ggdb -std=c++14 $(wordlist 1,2,$^) -o $@

.PHONY: clean

clean:
	rm -rf bin/*
