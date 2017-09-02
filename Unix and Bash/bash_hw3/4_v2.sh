#!/bin/bash
sed -n -r -e '
    H
    $ {
        x
        s/\\\n/!!!PLACEHOLDER_CONT!!!/g   # Erase line continuation.

        # Substitute `"..."` with `X...Y`.
        : del_quotations
        s#"(([^"\\]|\\[^"])*)"#!!!MISHA_X\1!!!_MISHA_Y!!!#
        t del_quotations

        # Repeatedly delete `//`, `\"`, `/*` and `*/` inside quotes.
        : del_something
        s#!!!MISHA_X!!!([^"\n]*)\\"([^"\n]*)!!!MISHA_Y!!!#!!!MISHA_X!!!\1!!!PLACEHOLDER_QUOTE!!!\2!!!MISHA_Y!!!#
        t del_something
        s#!!!MISHA_X!!!(.*?)//(.*?)!!!MISHA_Y!!!#!!!MISHA_X!!!\1!!!PLACEHOLDER_SLASH!!!\2!!!MISHA_Y!!!#
        t del_something
#        s#"([^"\n]*)/\*([^"\n]*)"#"\1!!!PLACEHOLDER_OPEN!!!\2"#
        t del_something
#        s#"([^"\n]*)\*/([^"\n]*)"#"\1!!!PLACEHOLDER_CLOSE!!!\2"#
        t del_something


        # Delete line comments: they are not inside quotes now.
#        s|//[^\n]*\n|\n|g

        # Delete multiline comments: they are not inside quotes now.
#        s|/\*.*\*/|\n|g

        p
    }
' main.c #| grep -oP '"([^"\\]|\\.)*"'
