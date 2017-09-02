#!/bin/bash

dir="$(readlink -f -- "${HOME}""/.au_cw_stack/")"
name="KOLTSOV_STACK"
fname="${dir}/${name}"
touch -- "$fname"

case "$1" in
    push)
        st="$2"
        printf "%s\n" "$st" >> "$fname"
        exit 0
        ;;
    pop)
        ln="$(cat -- "$fname" | wc -l)"
        if (( $ln == 0 ))
        then
            exit 1
        else
            sed -i -n -e '$!p' "$fname"
        fi
        exit 0
        ;;
    *)
        exit 1
esac
