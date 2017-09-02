all:
	g++ -std=c++11 -Wall -Werror -g ./src/mem.cpp -o ./bin/mem
	g++ -std=c++11 -g -Wall -Werror ./src/memtricky.cpp -o ./bin/memtricky
	g++ -std=c++11 -Wall -Werror -g ./src/profsys.cpp -o ./bin/profsys
	g++ -std=c++11 -Wall -Werror -g ./src/profus.cpp -o ./bin/profus

clean:
	rm -rf bin/*

.PHONY: all clean
