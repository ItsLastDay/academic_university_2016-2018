#!/bin/bash
<<< "$1" sed -r -e '
    :loop
    s#(.*?)(?<!*)\b(\w+)\b(?!*).*+##
    t loop
'
