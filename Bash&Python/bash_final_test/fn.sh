#!/bin/bash

last=.
i=0

cnt=0

while read f; do
    if (( $i == 0 ))
    then
       i=1
       printf ".\n"
       continue
    fi  

    pdir="$(dirname "$last")"
    curbase="$(basename "$f")"
    curdir="$(dirname "$f")"

    if (( ${#pdir} < ${#curdir} ))
    then
        cnt=$(expr $cnt + 2)
    else
        if (( ${#pdir} > ${#curdir} ))
        then
            cnt=$(expr $cnt - 2)
        fi
    fi

    printf "|-"
    for t in $(seq $cnt)
    do
        printf "-"
    done

    printf "%s\n" "$curbase"

    last="$f"
done < <(find)
