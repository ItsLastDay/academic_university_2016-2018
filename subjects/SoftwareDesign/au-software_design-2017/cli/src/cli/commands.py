"""Abstractions of commands and their results.

In principle, we have two types of commands:

    - a single command, like ``wc`` or ``cat``. It accepts
        input, returns something, etc.
        This is represented by :class:`.SingleCommand`.

    - a combination of commands, like ``pwd | wc``. It consists
        of several commands that interact with each other
        by some rules. This is represented by :class:`.ChainCommand`.

Since each command has the same interface (i.e. it can run,
given input and environment), the above classes share a
common base class, which represents an abstract command -
:class:`.RunnableCommand`.

This module also contains an abstraction of a command run
result.
"""
from abc import ABCMeta, abstractmethod
import logging
import copy

from cli.streams import OutputStream


class RunnableCommandResult:
    """Represents result of invoking a :class:`.RunnableCommand`.

    It is more convenient to have a single class than a bunch
    of values (output, return code, etc.).
    """

    def __init__(self, output_stream, new_env, ret_code):
        """ Create CommandResult out of <output_stream, new_env, ret_code>.

        `output_stream` (:module:`streams.OutputStream`): an output stream
            of the program. We convert
            it to InputStream in order
            to read later what the program wrote;
        `new_env` (:module:`environment.Environment`): a new environment
            in which shell should operate after executing the program.
            For example:
                ``x=1``
            should return a new environment with $x equal to `1`;
        `ret_code` (int): a return code.
        """
        self._input_stream = output_stream.to_input_stream()
        self._new_env = new_env
        self._ret_code = ret_code

    def get_return_code(self):
        """Getter for the return code"""
        return self._ret_code

    def get_output(self):
        """Read the command's output.

        Returns:
            str: a string representation of output.
        """
        return self._input_stream.get_input()

    def get_input_stream(self):
        """Getter for the input stream"""
        return self._input_stream

    def get_result_environment(self):
        """Getter for the new environment"""
        return self._new_env


class RunnableCommand(metaclass=ABCMeta):
    """An abstraction of every possible shell command (in broad sense).

    A command can be run.
    This is an abstract class.
    """

    @abstractmethod
    def run(self, input_stream, env):
        """Main action with the command: run it, given input and environment.

        Args:
            input_stream (:class:`streams.InputStream`): an input for this command;
            env (:class:`environment.Environment`): an environment in which command runs.

        Returns:
            :class:`.RunnableCommandResult`.
        """
        return NotImplemented


class CommandChain(RunnableCommand):
    """A subset of commands: those which take two commands and combine them.

    This is an abstract class.
    """

    def __init__(self, cmd1, cmd2):
        """Every CommandChain is constructed out of two commands.
        """
        self._cmd_left = cmd1
        self._cmd_right = cmd2


class CommandChainPipe(CommandChain):
    """Pipe: take one command's output and put into other command as input.

    The second command can ignore the input whatsoever.
    One can chain environment variable assignments using Pipe. All assignments
    will take place.

    If the first command fails (i.e. completes with non-zero status),
    then the result of Pipe is equal to the result of the first command
    (i.e. the second command is `not` run).

    Examples::
        cat test.txt | wc
        echo 123 | pwd
        x=1 | y=2
    """

    def run(self, input_stream, env):
        cmd_left_result = self._cmd_left.run(input_stream, env)

        if cmd_left_result.get_return_code():
            return cmd_left_result

        modified_input = cmd_left_result.get_input_stream()
        modified_env = cmd_left_result.get_result_environment()

        return self._cmd_right.run(modified_input, modified_env)


class SingleCommand(RunnableCommand):
    """A subset of commands: those which can execute on them own.

    This is an abstract class.
    """

    def __init__(self, args_lst):
        """Every SingleCommand is created out of arguments list.

        Example of arguments::
            python3 test.py xxx     -->    ['python3', 'test.py', 'xxx']
            echo "a b c d" 'wr tr$x' -->   ['echo', '"a b c d"', "'wr tr$x'"]
            x=1                     -->    ['x=1']

        Every descandant of this class describes which arguments it expects
        to see.
        """
        self._args_lst = args_lst


class CommandAssignment(SingleCommand):
    """An environment assignment.

    Modifies the environment, reassigning one variable's value.

    Command args:
        0 -- string of the form ``var=value``.

    Assignemnt of quoted strings is **not** supported, e.g.
    ``x="a b c"``.
    """

    def run(self, input_stream, env):
        output = OutputStream()
        return_code = 0
        new_env = copy.copy(env)

        equality_string = self._args_lst[0]
        logging.debug('Assignment {} is being applied.'.format(equality_string))

        first_eq_pos = equality_string.index('=')
        var_name, value = equality_string[:first_eq_pos], equality_string[first_eq_pos + 1:]
        new_env.set_var(var_name, value)
        return RunnableCommandResult(output, new_env, return_code)
