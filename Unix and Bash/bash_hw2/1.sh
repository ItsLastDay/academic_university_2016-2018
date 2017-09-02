#!/bin/bash
if (( $# != 2 )); then
    echo "Invalid number of parameters: need exactly 2"
    exit 1
fi

cat -- "$1" | xargs factor > "$2"
