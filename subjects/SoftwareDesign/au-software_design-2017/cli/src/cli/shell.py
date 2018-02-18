"""The main module that uses other modules.

This module has a class Shell which does
all the shell work:

    - read an input
    - call an appropriate module to preprocess input
    - call an appropriate module to lex input
    - call an appropriate module to parse lexemes
    - invoke the program represented by (sort of) AST.

"""
from cli.environment import Environment
from cli.streams import InputStream
from cli.lexer import Lexer
from cli.parser import Parser
from cli.preprocessor import Preprocessor
import cli.exceptions as exceptions


class Shell:
    """ The main shell REPL class.

    Has an :class:`environment.Environment` inside.

    Create an instance and run `main_loop`
    if you want to interact with user, like this::

        shell = Shell()
        shell.main_loop()

    """

    def __init__(self):
        """Create a Shell instance with empty environment"""
        self._env = Environment()

    def process_input(self, inp):
        """Take input string, parse it, run it.

        Args:
            inp (str): an input string

        Returns:
            :class:`commands.RunnableCommandResult`.
        """
        preprocessed_inp = Preprocessor.substitute_environment_variables(inp, self._env)
        lexemes = Lexer.get_lexemes(preprocessed_inp)
        runnable = Parser.build_command(lexemes)
        return runnable.run(InputStream(), self._env)

    def apply_command_result(self, command_result):
        """Take some programs result into account, i.e. change Shell's state.

        Change `self.env` according to `command_result`.
        This could be done in `main_loop` itself, but distinguishing
        this function makes testing easier.
        """
        self._env = command_result.get_result_environment()

    def main_loop(self):
        """Infinite loop: prompt user input, show command output.

        Reads from stdin, writes to stdout.
        Supports recovering from parsing and lexing errors.
        """
        user_asked_exit = False
        while not user_asked_exit:
            input_str = input('>')

            try:
                command_result = self.process_input(input_str)
                self.apply_command_result(command_result)
                print(command_result.get_output())

                ret_code = command_result.get_return_code()
                if ret_code != 0:
                    print('Process exited with error code {}'.format(ret_code))
            except exceptions.ParseException as ex:
                print('Parsing exception occured:\n{}'.format(str(ex)))
            except exceptions.LexException as ex:
                print('Lexing exception occured:\n{}'.format(str(ex)))
            except exceptions.ExitException:
                user_asked_exit = True

        print('Bye!')

