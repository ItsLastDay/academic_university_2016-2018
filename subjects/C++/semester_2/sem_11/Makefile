CPP = clang++
CPPFLAGS = -O0 -Wall -Werror -std=c++14 -DCONFIG_DEBUG=1 -DCONFIG_LOG_LEVEL=1

all: ./bin/main
	./bin/main


./bin/main: ./src/main.cpp ./src/myassert.h ./src/mylog.h ./src/instructions.h
	$(CPP) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm ./bin/*
