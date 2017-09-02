import sys
import argparse

from antlr4 import *
from LLexer import LLexer


class ExitErrorListener:
    # A listener that makes program exit with non-zero exit code.
    # (the default strategy is to print errors on console and proceed)
    # http://www.antlr.org/api/Java/org/antlr/v4/runtime/ANTLRErrorListener.html

    def syntaxError(self, recognizer,
            offendingSymbol, line, charPositionInLine, msg, exc):
        print('Syntax error at ({line}:{char}): {msg}'.format(line=line-1,
            char=charPositionInLine,
            msg=msg))
        sys.exit(1)


def calc_offsets(line_start, col_start, text):
    text_lines = text.splitlines()

    if len(text_lines) == 1:
        return line_start, col_start + len(text_lines[0]) - 1

    return line_start + len(text_lines) - 1, len(text_lines[-1]) - 1


def print_tokens(tokens, tok_type_to_name, need_filter):
    # `tokens` is a list of CommonToken from antlr.

    last_line = -1
    for token in tokens:
        if token.channel == token.HIDDEN_CHANNEL and need_filter:
            continue

        token.line -= 1

        tp_name = tok_type_to_name[token.type - 1]
        text = token.text
        col_start = token.column
        line_start = token.line

        line_end, col_end = calc_offsets(line_start, col_start, text)

        if last_line != line_start:
            last_line = line_start
            print('')

        print('{name}("{text}", ({line_st},{col_st}) : ({line_end},{col_end}))'.format(
                                                              name=tp_name,
                                                              text=text,
                                                              line_st=line_start,
                                                              col_st=col_start,
                                                              line_end=line_end,
                                                              col_end=col_end))

def main(argv):
    # Inspired by https://github.com/antlr/antlr4/blob/master/doc/python-target.md
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', help='file to lex')
    parser.add_argument('--filter', help='filter comments?', action='store_true')
    args = parser.parse_args()

    inp = FileStream(args.file_name)
    lexer = LLexer(inp)
    lexer.removeErrorListeners()
    lexer.addErrorListener(ExitErrorListener())

    tokens = lexer.getAllTokens()
    print_tokens(tokens, lexer.ruleNames, args.filter)


if __name__ == '__main__':
    sys.exit(main(sys.argv))
