import sys
import random
import string
import argparse
from collections import defaultdict
import os.path

from antlr4 import *
from parser_grammar.LLexer import LLexer
from parser_grammar.LParser import LParser
from parser_grammar.LParserListener import LParserListener


RESULTS_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'parse_results')

class ExitErrorListener(error.ErrorListener.ErrorListener):
    # A listener that makes program exit with non-zero exit code.
    # (the default strategy is to print errors on console and proceed)
    # http://www.antlr.org/api/Java/org/antlr/v4/runtime/ANTLRErrorListener.html

    def syntaxError(self, recognizer,
            offendingSymbol, line, charPositionInLine, msg, exc):
        print('Syntax error at ({line}:{char}): {msg}'.format(line=line-1,
            char=charPositionInLine,
            msg=msg))
        sys.exit(1)


def gen_random_name(prefix='', sz=8):
    return prefix + ''.join([random.choice(string.ascii_letters) for _ in range(sz)])


# Implementing visualization required deep diving into ANTLR4 source code:
# https://github.com/antlr/antlr4/tree/master/runtime/Python3/src/antlr4
class DotTreeListener(LParserListener):
    """ Convert ANTLR4 parse tree to DOT format."""
    def __init__(self):
        super().__init__()
        self.same_names_by_level = defaultdict(list)

        self.dot_repr = 'digraph parsetree {}\nrankdir=TB;\n'.format("{")


    def finalize_dot(self):
        for level in self.same_names_by_level.keys():
            nodes = self.same_names_by_level[level]
            self.dot_repr += '{} rank = same; {} {}\n'.format('{', '; '.join(nodes), '}')

        self.dot_repr += '{}'.format('}')


    def enterEveryRule(self, ctx):
        if ctx.stop is None:
            return
        dot_name = gen_random_name()
        ctx.dot_name = dot_name
        self.same_names_by_level[ctx.depth()].append(dot_name)

        rule_name = tree.Trees.Trees.getNodeText(ctx, recog=ctx.parser)
        cls = ctx.__class__.__name__
        # More specific name for rule (with labels).
        rule_name = '{} ({})'.format(rule_name, cls[:-len('Context')])

        start_idx = ctx.start.start
        stop_idx = ctx.stop.stop
        stream = ctx.start.getInputStream()
        original_program_text = stream.getText(start_idx, stop_idx)

        st_line, st_col = ctx.start.line - 1, ctx.start.column
        end_line, end_col = ctx.stop.line - 1, ctx.stop.column

        node_label = '{} [({}:{}), ({}:{})]'.format(rule_name, st_line, st_col, end_line, end_col)
        if len(ctx.children) > 1 or (len(ctx.children) == 1 and isinstance(ctx.children[0], TerminalNode)):
            node_label += '\\n{}'.format(original_program_text.replace('\r', '\\n').replace('\n', '\n').replace('\\', '\\\\').replace('"', '\\"'))

        self.dot_repr += 'node [ label = "{}" ]; {} ;\n'.format(node_label, dot_name)

        if ctx.parentCtx:
            self.dot_repr += '{} -> {} ;\n'.format(ctx.parentCtx.dot_name, dot_name)


def save_dot(dot_listener, filename):
    filename = os.path.basename(filename).replace('.', '_') + '.dot'
    with open(os.path.join(RESULTS_FOLDER, filename), 'w') as out:
        out.write(dot_listener.dot_repr)

def main(argv):
    # Inspired by https://github.com/antlr/antlr4/blob/master/doc/python-target.md
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', help='file to parse')
    args = parser.parse_args()

    inp = FileStream(args.file_name)
    lexer = LLexer(inp)
    lexer.removeErrorListeners()
    lexer.addErrorListener(ExitErrorListener())

    stream = CommonTokenStream(lexer)
    parser = LParser(stream)
    parser.removeErrorListeners()
    parser.addErrorListener(ExitErrorListener())

    parse_tree = parser.program()
    dot_listener = DotTreeListener()
    walker = ParseTreeWalker()
    walker.walk(dot_listener, parse_tree)

    dot_listener.finalize_dot()
    save_dot(dot_listener, args.file_name)


if __name__ == '__main__':
    sys.exit(main(sys.argv))
