"""Abstraction of shell environment"""
import pathlib
import os
import logging


class Environment:
    """A shell environment, in which commands run.

    It is a collection of pairs <var_name, var_value>
    plus a current directory.
    """

    def __init__(self):
        """Create an empty environment"""
        self._var_to_value = dict()
        self._current_working_directory = pathlib.Path(os.getcwd())

    def get_var(self, name):
        """Get variable value by name"""
        return self._var_to_value.get(name, '')

    def set_var(self, name, value):
        """Set variable value.

        Args:
            name (str): variable name.
            value (str): variable value (it should be string).
        """
        self._var_to_value[name] = value

    def get_cwd(self):
        """Get string representation of current working directory"""
        return str(self._current_working_directory)

    def set_cwd(self, dir_name):
        """Set current working directory.

        Do not check that the directory exists: we don't really care,
        it is just a string.

        Args:
            dir_name (str): string representation of a new working
                directory path.
        """
        dir_name = pathlib.Path(dir_name)
        new_path = self._current_working_directory.joinpath(dir_name)

        self._current_working_directory = new_path
        logging.info('Current working directory changed to {}'.format(new_path))

