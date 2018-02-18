#!/bin/bash

while (( $# > 0 )) 
do
    case $1 in
        -i)
            shift
            src_dir=$1
            ;;
        -o)
            shift
            dst_dir=$1
            ;;
        *) 
            echo "Invalid key" 1>&2
            exit 1
    esac

    shift
done

src_dir=$(readlink -f -- "${src_dir?"No source dir supplied"}")
dst_dir=$(readlink -f -- "${dst_dir?"No destination dir supplied"}")

find "$src_dir" -type f -executable -execdir cp --parents {} -t "$dst_dir" \;
