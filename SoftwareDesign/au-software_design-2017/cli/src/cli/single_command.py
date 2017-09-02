"""Concrete SingleCommand's and their Factory.

Since there can be many SingleCommand's (cat,
echo, wc, ..., you name it), it is convenient
to have them in a separate module.

There is a Factory class that "knows"
how to choose an appropriate Command
given it's string representation.
"""
import logging
import os.path
import os
import subprocess
import sys
import re
from docopt import docopt, DocoptExit
from schema import Schema, SchemaError, Optional, Use

from cli.commands import SingleCommand, RunnableCommandResult
from cli.exceptions import ExitException
from cli.streams import OutputStream


class CommandExternal(SingleCommand):
    """An external command (not described in shell).

    Command args:
        0 -- a command's name. This name is searched
            via joining the current working directory
            with the command name (i.e. it must be somewhere
            in the current directory).
        1..n -- command's arguments. There can be any arguments
            depending on a command.
    """

    COMMAND_NOT_FOUND = 1

    def run(self, input_stream, env):
        output = OutputStream()

        cmd_name = self._args_lst[0]
        cur_dir = env.get_cwd()
        cmd_name_full = os.path.join(cur_dir, cmd_name)

        modified_args = self._args_lst[:]
        modified_args[0] = cmd_name_full

        return_code = 0
        try:
            completed_process = subprocess.run(modified_args,
                                               input=input_stream.get_input(),
                                               stdout=subprocess.PIPE,
                                               encoding=sys.stdout.encoding)
            return_code = completed_process.returncode
            output.write(completed_process.stdout)
        except FileNotFoundError:
            output.write('Command {} not found.'.format(cmd_name_full))
            return_code = CommandExternal.COMMAND_NOT_FOUND

        return RunnableCommandResult(output, env, return_code)


class SingleCommandFactory:
    """A class that is responsible for building Single Commands.

    This class knows which commands exist in shell.
    """

    registered_commands = dict()

    @staticmethod
    def build_command(cmd_name_and_args_lexem_lst):
        """Build a single command out of list of lexemes representing it's arguments.

        Args:
            cmd_name_and_args_lexem_lst (list[:class:`lexer.Lexem`]): a list
                of lexemes. The first one must be STRING that represents a command
                name. The rest are STRING or QUOTED_STRING 's.

        All string representations of lexemes are passed to a corresponding
        SingleCommand descendant.
        """
        cmd_name = cmd_name_and_args_lexem_lst[0]
        cls = SingleCommandFactory._get_command_class_by_name(cmd_name.get_value())
        string_repr_of_all_args = [x.get_value() for x in cmd_name_and_args_lexem_lst]

        return cls(string_repr_of_all_args)


    @staticmethod
    def _get_command_class_by_name(cmd_name):
        cmd_cls = SingleCommandFactory.registered_commands.get(cmd_name, CommandExternal)
        logging.debug('SingleCommandFactory: ' \
                      'Class {} is responsible for invoking command {}'.format(
                          cmd_cls, cmd_name))
        return cmd_cls


def _register_single_command(command_name):
    """Every shell command with a name (e.g. `cat`) must be registered using this decorator.
    """

    def class_decorator(cls):
        """Take a class and put it into SingleCommandFactory's dictionary.
        """
        SingleCommandFactory.registered_commands[command_name] = cls
        return cls

    return class_decorator


@_register_single_command('echo')
class CommandEcho(SingleCommand):
    """`echo` command: prints it's arguments.

    Command args:
        0 -- `echo`

        1..n -- strings. Those strings are written to the output, space-separated.
            A new line is then written (as seen in bash).

    """

    def run(self, input_stream, env):
        output = OutputStream()
        output.write_line(' '.join(self._args_lst[1:]))

        return RunnableCommandResult(output, env, 0)


@_register_single_command('wc')
class CommandWc(SingleCommand):
    """`wc` command: count the number of words, characters and lines.

    Command args:
        0 -- `wc`

        1 (optional) -- a filename. If provided, then `wc` will
            count the number of characters in this file. Otherwise,
            it will take it's input.
            Can be relative or absolute.

    Returns `FILE_NOT_FOUND` exit code if file was provided,
    but was not found.
    """

    FILE_NOT_FOUND = 1
    BAD_NUMBER_OF_ARGS = 2

    @staticmethod
    def _get_num_words(input_str):
        num_words = 0
        last_was_char = False

        for ch in input_str:
            if not ch.isspace():
                num_words += not last_was_char
                last_was_char = True
            else:
                last_was_char = False

        return num_words

    @staticmethod
    def _wc_routine(input_str):
        num_lines = len(input_str.splitlines())
        num_words = CommandWc._get_num_words(input_str)
        num_bytes = len(input_str.encode(sys.stdin.encoding))

        wc_result = (num_lines, num_words, num_bytes)
        logging.debug('wc: result for "{}" is {}.'.format(input_str, wc_result))

        return wc_result

    def run(self, input_stream, env):
        return_code = 0
        output = OutputStream()
        wc_result = None

        num_args = len(self._args_lst)
        if num_args == 2:
            fl_name = self._args_lst[1]
            full_fl_name = os.path.join(env.get_cwd(), fl_name)

            if not os.path.isfile(full_fl_name):
                output.write('wc: file {} not found.'.format(full_fl_name))
                return_code = CommandWc.FILE_NOT_FOUND
            else:
                with open(full_fl_name, 'r') as opened_file:
                    wc_result = CommandWc._wc_routine(opened_file.read())
        elif num_args == 1:
            wc_result = CommandWc._wc_routine(input_stream.get_input())
        else:
            output.write('wc got wrong number of arguments: expected 0 or 1, '\
                         'got {}.'.format(num_args - 1))
            return_code = CommandWc.BAD_NUMBER_OF_ARGS

        if return_code == 0:
            n_lines, n_words, n_bytes = wc_result
            output.write('{} {} {}'.format(n_lines, n_words, n_bytes))

        return RunnableCommandResult(output, env, return_code)


@_register_single_command('cat')
class CommandCat(SingleCommand):
    """`cat` command: print it's input or file contents.

    Command args:
        0 -- `cat`

        1 (optional) -- a filename. If provided, `cat` will
            output that file's contents. Otherwise,
            it will print it's input.
            Can be relative or absolute.

    Returns `FILE_NOT_FOUND` exit code if file was provided,
    but was not found.
    """

    FILE_NOT_FOUND = 1
    BAD_NUMBER_OF_ARGS = 2

    def run(self, input_stream, env):
        return_code = 0
        output = OutputStream()

        num_args = len(self._args_lst)
        if num_args == 2:
            fl_name = self._args_lst[1]
            full_fl_name = os.path.join(env.get_cwd(), fl_name)

            if not os.path.isfile(full_fl_name):
                output.write('cat: file {} not found.'.format(full_fl_name))
                return_code = CommandCat.FILE_NOT_FOUND
            else:
                with open(full_fl_name, 'r') as opened_file:
                    output.write(opened_file.read())
        elif num_args == 1:
            output.write(input_stream.get_input())
        else:
            output.write('cat got wrong number of arguments: expected 0 or 1, '\
                         'got {}.'.format(num_args - 1))
            return_code = CommandCat.BAD_NUMBER_OF_ARGS

        return RunnableCommandResult(output, env, return_code)


@_register_single_command('pwd')
class CommandPwd(SingleCommand):
    """`pwd` command: print the current working directory.
    """

    BAD_NUMBER_OF_ARGS = 1

    def run(self, input_stream, env):
        output = OutputStream()

        if len(self._args_lst) != 1:
            output.write('pwd got wrong number of arguments: expected 0, '\
                         'got {}.'.format(len(self._args_lst) - 1))
            return_code = CommandPwd.BAD_NUMBER_OF_ARGS
            return RunnableCommandResult(output, env, return_code)

        cur_dir = env.get_cwd()
        output.write(cur_dir)

        return RunnableCommandResult(output, env, 0)

@_register_single_command('exit')
class CommandExit(SingleCommand):
    """`exit` command: exit shell.

    Performs exiting via throwing an exception,
    so all further commands are not run.
    """

    BAD_NUMBER_OF_ARGS = 1

    def run(self, input_stream, env):
        if len(self._args_lst) != 1:
            output = OutputStream()
            output.write('exit got wrong number of arguments: expected 0, '\
                         'got {}.'.format(len(self._args_lst) - 1))
            return_code = CommandExit.BAD_NUMBER_OF_ARGS
            return RunnableCommandResult(output, env, return_code)

        raise ExitException()


@_register_single_command('cd')
class CommandCd(SingleCommand):
    """`cd` command: change directory.

    Command args:
        0 -- `cd`
        1 -- a new filepath. Can be relative or absolute.

    Returns NEW_DIR_INVALID if the directory does not exist.
    Returns BAD_NUMBER_OF_ARGS if wrong number of arguments is supplied.
    """

    NEW_DIR_INVALID = 1
    BAD_NUMBER_OF_ARGS = 2

    def run(self, input_stream, env):
        output = OutputStream()
        return_code = 0

        if len(self._args_lst) != 2:
            output.write('cd got wrong number of arguments: expected 1, '\
                         'got {}.'.format(len(self._args_lst) - 1))
            return_code = CommandCd.BAD_NUMBER_OF_ARGS
            return RunnableCommandResult(output, env, return_code)

        cur_dir = env.get_cwd()
        new_dir = os.path.join(cur_dir, self._args_lst[1])
        logging.debug('cd: trying to change dir to {}.'.format(new_dir))

        if not os.path.isdir(new_dir):
            output.write('{} is not a directory.'.format(new_dir))
            return_code = CommandCd.NEW_DIR_INVALID
        else:
            env.set_cwd(new_dir)

        return RunnableCommandResult(output, env, return_code)


@_register_single_command('grep')
class CommandGrep(SingleCommand):
    """`grep` command: find strings in a file.

    Usage:
        grep [-A <n>] [-i] [-w] PATTERN FILE

    Arguments:
        PATTERN     regular expression pattern to search for
        FILE    path to file where the search is performed

    Options:
        -i               ignore case when searching
        -w               search for the whole word
        -A <n>           print n lines after match [default: 0]

    """


    def run(self, input_stream, env):
        output = OutputStream()
        return_code = 0

        schema = Schema({
            Optional('--help'): bool,
            '-A': Use(int, error='n should be integer'),
            Optional('-w'): bool,
            Optional('-i'): bool,
            'FILE': Use(lambda f: open(os.path.join(env.get_cwd(), f), 'r'), error='FILE should be readable'),
            'PATTERN': Use(re.compile, error='PATTERN must be compileable regexp')
        })

        parsed_args = None

        try:
            arguments = docopt(CommandGrep.__doc__, argv=self._args_lst[1:], help=False)
            parsed_args = schema.validate(arguments)
        except (SchemaError, DocoptExit) as e:
            output.write(CommandGrep.__doc__)
            logging.info('Grep arguments parse error: {}'.format(str(e)))
            return RunnableCommandResult(output, env, return_code)

        num_lines_after = parsed_args['-A']
        need_whole_words = parsed_args['-w']
        need_ignore_case = parsed_args['-i']
        decompiled_pattern = parsed_args['PATTERN'].pattern
        opened_file = parsed_args['FILE']

        re_flags = 0
        if need_ignore_case:
            re_flags |= re.IGNORECASE
        if need_whole_words:
            decompiled_pattern = '\\b{}\\b'.format(decompiled_pattern)

        compiled_pattern = re.compile(decompiled_pattern, re_flags)

        lines = opened_file.read().splitlines()
        for i, line in enumerate(lines):
            if re.search(compiled_pattern, line):
                for ln in lines[i:i+1+num_lines_after]:
                    output.write_line(ln)

        opened_file.close()

        return RunnableCommandResult(output, env, return_code)
