import unittest

from cli.exceptions import ParseException
from cli.parser import Parser
from cli.lexer import Lexem, LexemType
from cli.commands import SingleCommand, CommandChainPipe, CommandAssignment
from cli.single_command import CommandExternal, CommandExit, CommandCd, CommandCat, CommandPwd, CommandEcho, CommandWc


class ParserTest(unittest.TestCase):
    """A bunch of test for ensuring parsing correctness.
    """

    def test_single_command_no_args(self):
        lexems = [Lexem(LexemType.STRING, 'pwd', 0, 5)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandPwd)

    def test_single_command_with_args(self):
        lexems = [Lexem(LexemType.STRING, 'echo', 0, 3),
                  Lexem(LexemType.QUOTED_STRING, '"hello"', 5, 10),
                  Lexem(LexemType.STRING, 'world', 11, 15),
                  Lexem(LexemType.QUOTED_STRING, "'hello'", 16, 20)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandEcho)

    def test_external_command_no_args(self):
        lexems = [Lexem(LexemType.STRING, 'some_command', 0, 10)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandExternal)

    def test_external_command_args(self):
        lexems = [Lexem(LexemType.STRING, 'python3', 0, 7),
                  Lexem(LexemType.STRING, '-c', 8, 10),
                  Lexem(LexemType.QUOTED_STRING, "'print(\"hello\")'", 11, 20)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandExternal)

    def test_assignment(self):
        lexems = [Lexem(LexemType.ASSIGNMENT, 'x=1', 0, 3)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandAssignment)

    def test_pipe_two_cmd(self):
        lexems = [Lexem(LexemType.STRING, 'echo', 0, 4),
                  Lexem(LexemType.ASSIGNMENT, 'yyy=123', 5, 10),
                  Lexem(LexemType.PIPE, '|', 12, 12),
                  Lexem(LexemType.STRING, 'wc', 14, 15)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandChainPipe)

    def test_command_wc(self):
        lexems = [Lexem(LexemType.STRING, 'wc', 0, 2)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandWc)

    def test_command_exit(self):
        lexems = [Lexem(LexemType.STRING, 'exit', 0, 2)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandExit)

    def test_command_pwd(self):
        lexems = [Lexem(LexemType.STRING, 'pwd', 0, 2)]

        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandPwd)

    def test_command_echo(self):
        lexems = [Lexem(LexemType.STRING, 'echo', 0, 2),
                  Lexem(LexemType.STRING, '1', 4, 5)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandEcho)

    def test_command_cat(self):
        lexems = [Lexem(LexemType.STRING, 'cat', 0, 2),
                  Lexem(LexemType.STRING, '1.txt', 4, 5)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandCat)

    def test_command_cd(self):
        lexems = [Lexem(LexemType.STRING, 'cd', 0, 2),
                  Lexem(LexemType.STRING, '..', 4, 5)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandCd)

    def test_assignment_pipe(self):
        lexems = [Lexem(LexemType.ASSIGNMENT, 'x=1', 0, 3),
                  Lexem(LexemType.PIPE, '|', 4, 4),
                  Lexem(LexemType.STRING, 'pwd', 6, 8)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandChainPipe)

    def test_triple_pipe(self):
        lexems = [Lexem(LexemType.STRING, 'pwd', 0, 4),
                  Lexem(LexemType.PIPE, '|', 5, 5),
                  Lexem(LexemType.STRING, 'wc', 6, 7),
                  Lexem(LexemType.PIPE, '|', 8, 8),
                  Lexem(LexemType.STRING, 'wc', 9, 10)]
        runnable = Parser.build_command(lexems)
        self.assertEqual(type(runnable), CommandChainPipe)

    def test_smth_after_assignment(self):
        lexems = [Lexem(LexemType.ASSIGNMENT, 'x=1', 0, 3),
                  Lexem(LexemType.STRING, 'blabla', 4, 7)]
        self.assertRaises(ParseException, Parser.build_command, lexems)

    def test_empty_after_pipe(self):
        lexems = [Lexem(LexemType.ASSIGNMENT, 'pwd', 0, 3),
                  Lexem(LexemType.PIPE, '|', 4, 4)]
        self.assertRaises(ParseException, Parser.build_command, lexems)

    def test_empty_between_pipes(self):
        lexems = [Lexem(LexemType.ASSIGNMENT, 'exit', 0, 3),
                  Lexem(LexemType.PIPE, '|', 4, 4),
                  Lexem(LexemType.PIPE, '|', 5, 5),
                  Lexem(LexemType.STRING, 'wc', 6, 7)]
        self.assertRaises(ParseException, Parser.build_command, lexems)
