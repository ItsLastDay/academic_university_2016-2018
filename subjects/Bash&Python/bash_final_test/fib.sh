#!/bin/bash

n="$1"
a=0
b=1

if (( $n == 0 ))
then
    printf "0"
else
    if (( $n == 1 ))
    then
        printf "1"
    fi
fi

n=$(expr $n - 1)

for i in $(seq $n)
do
    tmp=$(echo "${a} + ${b}" | bc)
    a=$b
    b=$tmp
done

printf "%s\n" "$b"
