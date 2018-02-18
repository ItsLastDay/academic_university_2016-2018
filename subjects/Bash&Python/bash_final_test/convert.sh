#!/bin/bash
inp=$1
outp=$2
num=$3

echo "ibase=${inp}; obase=${outp}; ${num}" | bc
