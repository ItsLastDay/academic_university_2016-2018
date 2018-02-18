CC=g++
CFLAGS=-c -Wall -Werror
EXEC=main

all: ${EXEC}

${EXEC}: hello.o main.o
	${CC} hello.o main.o -o ${EXEC}

main.o:
	${CC} ${CFLAGS} main.cpp -I ./hello_include

hello.o:
	${CC} ${CFLAGS} -c hello.cpp


clean:
	rm *.o ${EXEC}
