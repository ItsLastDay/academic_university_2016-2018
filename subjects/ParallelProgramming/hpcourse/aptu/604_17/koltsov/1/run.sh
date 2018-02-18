#!/bin/bash
./compile.sh
i=0
while true 
do
    i=$((i+1))
    x=$(./result < input_large)
    if [ $x != "4000" ]
    then
        echo "BAD"
        break
    else
        echo "OK" $i
    fi
done
