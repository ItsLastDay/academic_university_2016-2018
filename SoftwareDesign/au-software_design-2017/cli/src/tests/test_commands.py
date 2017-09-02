import unittest
import os
import os.path
import sys

from cli.exceptions import ExitException
from cli.commands import CommandChainPipe, CommandAssignment
from cli.single_command import CommandExternal, CommandExit, CommandCd, CommandCat, CommandPwd, CommandEcho, CommandWc, SingleCommandFactory, CommandGrep
from cli.lexer import Lexem, LexemType
from cli.environment import Environment
from cli.streams import InputStream


BASE_DIR = os.path.dirname(os.path.abspath(__file__))
wc_file_path = '"{}"'.format(os.path.join(BASE_DIR, 'wc_file.txt'))
grep_file_path = '"{}"'.format(os.path.join(BASE_DIR, 'grep_test_file.txt'))


class CommandsTest(unittest.TestCase):
    """Functionality test for all descdendants of RunnableCommand.
    """

    def setUp(self):
        self.init_env = Environment()
        self.init_input = InputStream()
        self.build_cmd = SingleCommandFactory.build_command

    def test_assignment(self):
        cmd = CommandAssignment([Lexem(LexemType.ASSIGNMENT, 'qwerty=123', 0, 10).get_value()])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_result_environment().get_var('qwerty'), '123')
        self.assertEqual(cmd_result.get_output(), '')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_external_command(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, sys.executable, 0, 7),
                                 Lexem(LexemType.STRING, '-c', 8, 10),
                                 Lexem(LexemType.QUOTED_STRING, 
                                       "'print(\"hello\", end='')'", 11, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), 'hello')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_external_command_nonexistant(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'some_ololo_command', 0, 7)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_return_code(), CommandExternal.COMMAND_NOT_FOUND)

    def test_exit_command(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'exit', 0, 3)])
        
        self.assertRaises(ExitException, cmd.run, self.init_input, self.init_env)

    def test_cd_command(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'cd', 0, 1),
                         Lexem(LexemType.STRING, os.pardir, 3, 5)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        real_parent_dir = os.path.join(os.getcwd(), os.pardir)

        cmd = self.build_cmd([Lexem(LexemType.STRING, 'pwd', 0, 3)])
        cmd_result = cmd.run(self.init_input, self.init_env)
        
        self.assertEqual(cmd_result.get_output(), real_parent_dir)
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_cat_command(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'cat', 0, 2),
                          Lexem(LexemType.QUOTED_STRING, wc_file_path, 4, 10)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), 
                         'this is a word.\nA line.\n')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_cat_command_from_stdin(self):
        cmd_1 = self.build_cmd([Lexem(LexemType.STRING, 'pwd', 0, 3)])
        cmd_2 = self.build_cmd([Lexem(LexemType.STRING, 'cat', 5, 7)])
        cmd = CommandChainPipe(cmd_1, cmd_2)
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), os.getcwd())
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_wc_cat_nonexistant_file(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'cat', 0, 3),
                          Lexem(LexemType.STRING, 'some_dummy_file_qqqq', 4, 15)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_return_code(), CommandCat.FILE_NOT_FOUND)


        cmd = self.build_cmd([Lexem(LexemType.STRING, 'wc', 0, 3),
                          Lexem(LexemType.STRING, 'some_dummy_file_qqqq', 4, 15)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_return_code(), CommandWc.FILE_NOT_FOUND)

    def test_echo_command(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'echo', 0, 3),
                           Lexem(LexemType.QUOTED_STRING, '"bla bla    bla"', 4, 15)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), 'bla bla    bla{}'.format(os.linesep))
        self.assertEqual(cmd_result.get_return_code(), 0)

        cmd = self.build_cmd([Lexem(LexemType.STRING, 'echo', 0, 3),
                           Lexem(LexemType.QUOTED_STRING, '"1"', 4, 6),
                           Lexem(LexemType.STRING, '234', 7, 10)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), '1 234{}'.format(os.linesep))
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_wc_command(self):
        cmd_1 = self.build_cmd([Lexem(LexemType.STRING, 'echo', 0, 3),
                           Lexem(LexemType.QUOTED_STRING, '"5 c 123    qwe"', 4, 15)])
        cmd_2 = self.build_cmd([Lexem(LexemType.STRING, 'wc', 16, 18)])
        cmd = CommandChainPipe(cmd_1, cmd_2)
        cmd_result = cmd.run(self.init_input, self.init_env)

        exp_length = 14 + len(os.linesep)
        self.assertEqual(cmd_result.get_output(), '1 4 {}'.format(exp_length))
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_wc_command_file(self):
        cmd = self.build_cmd([Lexem(LexemType.STRING, 'wc', 0, 3),
                         Lexem(LexemType.QUOTED_STRING, wc_file_path, 4, 15)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), '2 6 24')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_pipe_two_cmd(self):
        cmd_1 = self.build_cmd([Lexem(LexemType.STRING, 'echo', 0, 4),
                             Lexem(LexemType.ASSIGNMENT, 'yyy=123', 5, 10)])
        cmd_2 = self.build_cmd([Lexem(LexemType.STRING, 'wc', 14, 15)])
        cmd = CommandChainPipe(cmd_1, cmd_2)
        cmd_result = cmd.run(self.init_input, self.init_env)

        exp_length = 7 + len(os.linesep)
        self.assertEqual(cmd_result.get_output(), '1 1 {}'.format(exp_length))
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_pipe_three_cmd(self):
        cmd_1 = self.build_cmd([Lexem(LexemType.STRING, 'echo', 0, 4),
                             Lexem(LexemType.QUOTED_STRING, "'hello$x'", 5, 15)])
        cmd_2 = self.build_cmd([Lexem(LexemType.STRING, 'wc', 17, 18)])
        cmd_3 = self.build_cmd([Lexem(LexemType.STRING, 'wc', 19, 20)])
        pipe_1_2 = CommandChainPipe(cmd_1, cmd_2)
        pipe_1_2_3 = CommandChainPipe(pipe_1_2, cmd_3)
        cmd_result = pipe_1_2_3.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output(), '1 3 5')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_pipe_threeway_ignore_first_cmd(self):
        cmd_1 = self.build_cmd([Lexem(LexemType.STRING, 'pwd', 0, 4)])
        cmd_2 = self.build_cmd([Lexem(LexemType.STRING, 'echo', 5, 8),
                           Lexem(LexemType.STRING, 'qQqQ', 9, 12)])
        cmd_3 = self.build_cmd([Lexem(LexemType.STRING, 'wc', 13, 15)])

        pipe_1_2 = CommandChainPipe(cmd_1, cmd_2)
        pipe_1_2_3 = CommandChainPipe(pipe_1_2, cmd_3)
        cmd_result = pipe_1_2_3.run(self.init_input, self.init_env)

        exp_length = 4 + len(os.linesep)
        self.assertEqual(cmd_result.get_output(), '1 1 {}'.format(exp_length))
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_piped_assignment(self):
        cmd_1 = CommandAssignment([Lexem(LexemType.ASSIGNMENT, 
                                       'tEq=1', 0, 5).get_value()])
        cmd_2 = CommandAssignment([Lexem(LexemType.ASSIGNMENT, 
                                       'x=a', 6, 15).get_value()])
        cmd = CommandChainPipe(cmd_1, cmd_2)
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_result_environment().get_var('tEq'), '1')
        self.assertEqual(cmd_result.get_result_environment().get_var('x'), 'a')
        self.assertEqual(cmd_result.get_output(), '')
        self.assertEqual(cmd_result.get_return_code(), 0)

    def test_grep_plain(self):
        cmd = SingleCommandFactory.build_command([Lexem(LexemType.STRING, 'grep', 0, 4),
                           Lexem(LexemType.STRING, 'all', 5, 7),
                           Lexem(LexemType.QUOTED_STRING, grep_file_path, 8, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output().strip(), 'really cool{}all'.format(os.linesep))

    def test_grep_whole_word(self):
        cmd = SingleCommandFactory.build_command([Lexem(LexemType.STRING, 'grep', 0, 4),
                           Lexem(LexemType.STRING, '-w', 4, 5),
                           Lexem(LexemType.STRING, 'all', 5, 7),
                           Lexem(LexemType.QUOTED_STRING, grep_file_path, 8, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output().strip(), 'all')

    def test_grep_nofile(self):
        cmd = SingleCommandFactory.build_command([Lexem(LexemType.STRING, 'grep', 0, 4),
                           Lexem(LexemType.STRING, 'hello', 5, 7),
                           Lexem(LexemType.QUOTED_STRING, "some_file", 8, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output().strip()[:10], CommandGrep.__doc__[:10])

    def test_grep_case_insens(self):
        cmd = SingleCommandFactory.build_command([
                           Lexem(LexemType.STRING, 'grep', 0, 4),
                           Lexem(LexemType.STRING, '-i', 4, 5),
                           Lexem(LexemType.STRING, 'bye', 5, 7),
                           Lexem(LexemType.QUOTED_STRING, grep_file_path, 8, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output().strip(), 'bye world{}BYE world'.format(os.linesep))

    def test_grep_more_lines(self):
        cmd = SingleCommandFactory.build_command([
                           Lexem(LexemType.STRING, 'grep', 0, 4),
                           Lexem(LexemType.STRING, '-A 2', 4, 5),
                           Lexem(LexemType.STRING, 'mega', 5, 7),
                           Lexem(LexemType.QUOTED_STRING, grep_file_path, 8, 20)])
        cmd_result = cmd.run(self.init_input, self.init_env)

        self.assertEqual(cmd_result.get_output().strip(), 'for mega-grep{}really cool{}so much'.format(os.linesep, os.linesep))
