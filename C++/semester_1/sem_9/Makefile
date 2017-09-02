CPP = g++
CPPFLAGS = -Wall -Werror -O2 --std=c++11


all: ./bin/result
	valgrind --leak-check=full ./bin/result

./bin/my_ostream_con.o: ./src/my_ostream_con.cpp ./src/my_ostream_con.h ./src/my_ostream.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/my_ostream_file.o: ./src/my_ostream_file.cpp ./src/my_ostream_file.h ./src/my_ostream.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/my_ostream_combo.o: ./src/my_ostream_combo.cpp ./src/my_ostream_combo.h ./src/my_ostream.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/main.o: ./src/main.cpp ./src/my_ostream_con.h ./src/my_ostream_file.h ./src/my_ostream_combo.h
	$(CPP) $(CPPFLAGS) -c $< -o $@

./bin/result: ./bin/main.o ./bin/my_ostream_con.o ./bin/my_ostream_file.o ./bin/my_ostream_combo.o
	$(CPP) $(CPPFLAGS) $^ -o $@ 

clean:
	rm -rf ./bin

.PHONY: clean all
