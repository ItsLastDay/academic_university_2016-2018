#!/usr/bin/env python3

import sys


def get_number_of_words(line):
    num_words = 0
    last_was_char = False
    for c in line:
        if not c.isspace():
            num_words += not last_was_char
            last_was_char = True
        else:
            last_was_char = False

    return num_words


def main():
    num_lines, num_words, num_bytes = 0, 0, 0
    for line in sys.stdin:
        num_lines += 1
        num_bytes += len(line.encode(sys.stdin.encoding))
        num_words += get_number_of_words(line)

    # `wc` output width is different for different types of files:

    # Example 1: input from a file, width is computed.
    # wc < wc.py
    #  29  64 628

    # Example 2: input from a pipe, width is fixed to 7.
    # cat wc.py | wc
    #      29      64     628

    # Try to mimic the above behaviour:
    width = 7
    if sys.stdin.seekable():
        width = len(str(num_bytes))

    fmt_string = '{line:{width}} {word:{width}} {byte:{width}}'
    print(fmt_string.format(line=num_lines,
                            word=num_words, byte=num_bytes, width=width))

if __name__ == '__main__':
    main()
