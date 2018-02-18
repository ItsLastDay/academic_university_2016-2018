""" A module with Preprocessor responsibility.

This module holds the Preprocessor - an entity
that accepts raw string as input, and expands
variables (in the form ``$x``) according to
an environment.

Preprocessing is a common action in programming languages,
so we use it in interpreting Shell commands as well.
"""
import logging
import enum


@enum.unique
class _PreprocessorAutomataState(enum.Enum):
    INITIAL_STATE = 1
    INSIDE_DOUBLE_QUOTES = 2
    INSIDE_SINGLE_QUOTES = 3
    MET_DOLLAR = 4
    READING_VAR_NAME = 5
    MET_DOLLAR_INSIDE_DOUBLE_QUOTES = 6
    READING_VAR_NAME_INSIDE_DOUBLE_QUOTES = 7


class Preprocessor:
    """A static class for preprocessing a shell input string.

    Given a raw string, we want to preprocess it, i.e.
    substitue things like `$x` into previously
    assigned value of `x`.
    """

    @staticmethod
    def substitute_environment_variables(raw_str, env):
        """Do a one-time pass over string and substitute `$x`-like patterns.

        Args:
            raw_str (str): an initial, unprocessed string;

            env (:class:`environment.Environment`): an environment
                in which this string must be expanded.

        Returns:
            str. The processed string.
            All substrings in single quotes are left untouched.

            Inside double quotes, nonempty substrings starting with `$` sign and
            ending in

                - space symbol
                - double or single quotes
                - `$` sign

            are treated as variable names. The values for these
            variables are queried from the input `env`.

            Outside any quotation, similar rules apply: nonempty substrings
            that start with `$` and end either

                - before the next space character
                - before the other `$` sign
                - at the end of the input string
                - at the beginning of quotes (single or double)

            are treated as variable names.

        Example:
            If the environment contains::

                x=1
                long_name=qwe

            Then the following substitutions apply (nonexistant variables
            are substituted by an empty string)::

                echo "123$x"    -->         echo "1231"
                echo "123$x "    -->        echo "1231 "
                echo "123$xy "    -->       echo "123 "
                echo "123$x dfg"    -->     echo "1231 dfg"
                echo $long_name'123'  -->   echo qwe'123'
                echo $long_name2'123'  -->  echo '123'
                echo $x '123'  -->          echo 1 '123'
                echo $x"qwe"    -->         echo 1"qwe"
                echo $x$long_name  -->      echo 1qwe
                echo `$x`"$x"  -->          echo `$x`"1"
        """
        processed_prefix = ''
        unprocessed_suffix = raw_str
        autom_state = _PreprocessorAutomataState.INITIAL_STATE

        while unprocessed_suffix:
            next_var_info = Preprocessor._find_next_var(unprocessed_suffix, autom_state)

            if next_var_info:
                (nonvar_part, var_name,
                 unprocessed_part, autom_state) = next_var_info
                processed_prefix += nonvar_part
                processed_prefix += env.get_var(var_name)

                unprocessed_suffix = unprocessed_part
            else:
                processed_prefix += unprocessed_suffix
                unprocessed_suffix = ''

        logging.info('Preprocessor: """{}""" processed ' \
                     'to """{}"""'.format(raw_str, processed_prefix))
        return processed_prefix

    @staticmethod
    def _find_next_var(raw_str, autom_state):
        """Return information about the leftmost variable in the string.

        Args:
            raw_str (str): an unprocessed string;
            autom_state (_PreprocessorAutomataState): a current state
            of the automata.

        Returns:
            If `raw_str` contains a variable, return tuple
             <part of the string before the variable,
              variable name,
              part of the string after the variable,
              automata state>

            Otherwise, return None.
        """
        read_idx = 0
        sz_raw = len(raw_str)
        var_name = ''

        while read_idx < sz_raw:
            cur_char = raw_str[read_idx]

            if autom_state == _PreprocessorAutomataState.INITIAL_STATE:
                if cur_char == '"':
                    autom_state = _PreprocessorAutomataState.INSIDE_DOUBLE_QUOTES
                elif cur_char == "'":
                    autom_state = _PreprocessorAutomataState.INSIDE_SINGLE_QUOTES
                elif cur_char == '$':
                    autom_state = _PreprocessorAutomataState.MET_DOLLAR
                else:
                    pass
            elif autom_state == _PreprocessorAutomataState.INSIDE_DOUBLE_QUOTES:
                if cur_char == '"':
                    autom_state = _PreprocessorAutomataState.INITIAL_STATE
                elif cur_char == '$':
                    autom_state = _PreprocessorAutomataState.MET_DOLLAR_INSIDE_DOUBLE_QUOTES
                else:
                    pass
            elif autom_state == _PreprocessorAutomataState.INSIDE_SINGLE_QUOTES:
                if cur_char == "'":
                    autom_state = _PreprocessorAutomataState.INITIAL_STATE
                else:
                    pass
            elif autom_state == _PreprocessorAutomataState.MET_DOLLAR:
                if cur_char == '"':
                    autom_state = _PreprocessorAutomataState.INSIDE_DOUBLE_QUOTES
                elif cur_char == "'":
                    autom_state = _PreprocessorAutomataState.INSIDE_SINGLE_QUOTES
                elif cur_char == '$':
                    pass
                elif cur_char.isspace():
                    autom_state = _PreprocessorAutomataState.INITIAL_STATE
                else:
                    var_name = cur_char
                    autom_state = _PreprocessorAutomataState.READING_VAR_NAME
            elif autom_state == _PreprocessorAutomataState.READING_VAR_NAME:
                if cur_char in ('"', "'", '$') or cur_char.isspace():
                    return (raw_str[:read_idx - len(var_name) - 1],
                            var_name,
                            raw_str[read_idx:],
                            _PreprocessorAutomataState.INITIAL_STATE)
                else:
                    var_name += cur_char
            elif autom_state == _PreprocessorAutomataState.MET_DOLLAR_INSIDE_DOUBLE_QUOTES:
                if cur_char == '"':
                    autom_state = _PreprocessorAutomataState.INITIAL_STATE
                elif cur_char == '$':
                    pass
                elif cur_char.isspace():
                    autom_state = _PreprocessorAutomataState.INSIDE_DOUBLE_QUOTES
                else:
                    var_name = cur_char
                    autom_state = _PreprocessorAutomataState.READING_VAR_NAME_INSIDE_DOUBLE_QUOTES
            elif autom_state == _PreprocessorAutomataState.READING_VAR_NAME_INSIDE_DOUBLE_QUOTES:
                if cur_char in ('"', "'", '$') or cur_char.isspace():
                    return (raw_str[:read_idx - len(var_name) - 1],
                            var_name,
                            raw_str[read_idx:],
                            _PreprocessorAutomataState.INSIDE_DOUBLE_QUOTES)
                else:
                    var_name += cur_char

            read_idx += 1

        # Automata can't be in `READING_VAR_NAME_INSIDE_DOUBLE_QUOTES` state
        # if the input string is valid. Preprocessor won't check for validity.
        if autom_state == _PreprocessorAutomataState.READING_VAR_NAME:
            return (raw_str[:read_idx - len(var_name) - 1],
                    var_name,
                    '',
                    _PreprocessorAutomataState.INITIAL_STATE)

        return None
