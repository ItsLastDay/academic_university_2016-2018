#!/bin/bash
sed -n -r -e '
    H
    $ {
        x
        s/\\\n/!!!PLACEHOLDER_CONT!!!/g   # Erase line continuation.

        # Repeatedly delete `//`, `\"`, `/*` and `*/` inside quotes.
        : del_something
        s#"([^"\n]*)//([^"\n]*)"#"\1!!!PLACEHOLDER_SLASH!!!\2"#
        t del_something
        s#"([^"\n]*)\\"([^"\n]*)"#"\1!!!PLACEHOLDER_QUOTE!!!\2"#
        t del_something
        s#"([^"\n]*)/\*([^"\n]*)"#"\1!!!PLACEHOLDER_OPEN!!!\2"#
        t del_something
        s#"([^"\n]*)\*/([^"\n]*)"#"\1!!!PLACEHOLDER_CLOSE!!!\2"#
        t del_something

        # Delete line comments: they are not inside quotes now.
        s|//[^\n]*\n|\n|g

        # Delete multiline comments: they are not inside quotes now.
        s|/\*.*\*/|\n|g

        p
    }
' main.c #| grep -oP '"([^"\\]|\\.)*"'
