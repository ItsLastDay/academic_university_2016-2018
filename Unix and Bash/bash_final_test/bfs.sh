#!/bin/bash

i=0

while (( 1 ))
do
    res="$(find . -maxdepth "$i" -mindepth "$i" -printf "%f\n")"

    if [[ $res ]]
    then
        printf "%s" "$res" | sort
        printf "\n"
    else
        break
    fi

    i=$(expr $i + 1)
done
