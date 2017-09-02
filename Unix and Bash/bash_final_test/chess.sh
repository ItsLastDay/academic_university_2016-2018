#!/bin/bash
n=$1

for i in $(seq $n)
do
    for j in $(seq $n)
    do
        if (( (i + j) % 2 == 0 ))
        then
            printf " "
        else
            printf "*"
        fi
    done
    
    printf "\n"
done
