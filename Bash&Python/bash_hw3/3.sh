#!/bin/bash
ifconfig | sed -n -r -e '
    /^$/ { 
        $!s|^$|---------|
        p
        b
    }

    /^$/ !{
        : mask_loop
        /(([0-9]){1,3}\.){3}([0-9]){1,3}/ {
            s/(([0-9]){1,3}\.){3}([0-9]){1,3}/\
&\
/
            t inner_loop
            b done
            : inner_loop
            P
            s/^[^\n]*+\n//
            h
            y/0123456789/xxxxxxxxxx/
            P
            x
            s/^[^\n]*+\n//
            s/\n+$//
            b mask_loop
        }

        : done
        p
    }
' | sed -n -r -e '
    H
    $ {
        x
        s/\n((x{1,3}\.){3}x{1,3})\n/\1/g
        s/\n{2,}/\n/g
        p    
    }
'
