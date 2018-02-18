#/bin/bash

make
while \
valgrind --error-exitcode=1 --leak-check=full ./bin/test; do :; done
