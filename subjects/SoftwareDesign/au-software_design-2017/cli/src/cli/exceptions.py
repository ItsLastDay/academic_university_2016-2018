"""A container for all shell-related exceptions"""


class ShellException(Exception):
    """A base class for all shell exceptions"""

    pass


class ParseException(ShellException):
    """An exception that occurs during parsing"""

    pass


class LexException(ShellException):
    """An exception that occurs during lexing"""

    pass


class ExitException(ShellException):
    """This exception is raised when `exit` command is executed"""

    pass
