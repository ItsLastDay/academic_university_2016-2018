#!/bin/bash
sed -n -r -e '
    H
    $ {
        x

        # Erase line continuation.
        s/\\\n/!!!PLACEHOLDER_CONT!!!/g  

        : del_something
        s#"([^"\n]*)\\"([^"\n]*)"#"\1!!!PLACEHOLDER_QUOTE!!!\2"#
        t del_something

        : dissimetry
        s#"([^"\n]*)"#\1#
        t dissimetry

        : del_quote_com
        s#([^]*)//([^]*)#\1!!!PLACEHOLDER_SLASH!!!\2#
        t del_quote_com
        s#([^]*)/\*([^]*)#\1!!!PLACEHOLDER_START!!!\2#
        t del_quote_com
        s#([^]*)\*/([^]*)#\1!!!PLACEHOLDER_END!!!\2#
        t del_quote_com

        # Delete line comments: they are not inside quotes now.
        s|//[^\n]*\n|\n|g

        # Delete multiline comments: they are not inside quotes now.
        s#/\*([^*]|\*[^/])*\*/#\n#g

        # Delete everything except ^A...^B contents, with \n-s in between.
        s|\n||
        s#[^]*#\n#g
        s#^[^]*##
        s#[^]*$##

        # Reconstruct initial notation.
        s//"/g
        s//"/g
        s/!!!PLACEHOLDER_QUOTE!!!/\\"/g
        s#!!!PLACEHOLDER_SLASH!!!#//#g
        s#!!!PLACEHOLDER_START!!!#/*#g
        s#!!!PLACEHOLDER_END!!!#*/#g
        s#!!!PLACEHOLDER_CONT!!!#\\\
#g

        p
    }
' main.c 
