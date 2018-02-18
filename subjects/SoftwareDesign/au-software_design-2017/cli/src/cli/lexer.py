"""A module with Lexer responsibility.

Lexing is a common step in language
compilation or interpreting. A lexem
is a group of tokens of the input
stream, grouped by some "meaning".
"""
import enum
import logging

from cli.exceptions import LexException


@enum.unique
class LexemType(enum.Enum):
    """Possible types of Lexems that Lexer produces.

    These are possible lexemes:

       - ``QUOTED_STRING`` is a sequence of characters inside double
            or single quotes;
       - ``STRING`` is a non-space sequence of characters;
       - ``ASSIGNMENT`` is a string of the form "smth=smth_other" (without quotes),
            `smth_other` can be empty;
       - ``PIPE`` is a `|` symbol.

    """

    QUOTED_STRING = 1
    STRING = 2
    ASSIGNMENT = 3
    PIPE = 4


class Lexem:
    """A single lexem.

    A Lexem provides interface for querying it's
    position in the string (for producing meaningful error
    messages), it's type and for getting it's string representation.
    """

    def __init__(self, tp, val, start_idx, end_idx):
        """Create a lexem out of type, string, start and end indices.

        Args:
            tp (:class:`.LexemType`): a type of lexem;
            val (str): a string representation of this lexem;
            start_idx (int): starting index, zero-based;
            end_idx (int): ending index, zero-based.
        """
        self._tp = tp
        self._val = val
        self._start_idx = start_idx
        self._end_idx = end_idx

    def get_value(self):
        """Return string representation of this lexem.

        Quotes are stripped from QUOTED_STRING.
        """
        if self.get_type() == LexemType.QUOTED_STRING:
            return self._val[1:-1]

        return self._val

    def get_type(self):
        """Return type of this lexem.
        """
        return self._tp

    def get_position(self):
        """Return string representation of the position.

        For example, if ``self._start_idx = 1`` and ``self.end_idx = 5``,
        then this function will return ``(1:5)``.
        """
        return '({}:{})'.format(self._start_idx, self._end_idx)


class Lexer:
    """A static class for lexing a preprocessed string.

    Given a string, we want to split it into meaningful
    (more or less) tokens.
    Possible tokens are described in :class:`.LexemType` docstring.
    """

    @staticmethod
    def get_lexemes(raw_str):
        """Scan the string left-to-right, output list of lexemes.

        Args:
            raw_str (str): a string to lex.

        Returns:
            list[:class:`.Lexem`] -- the resulting lexemes.

        Raises:
            :class:`exceptions.LexException`: if some quoted string started but never ends.
        """
        lexem_list = []
        unprocessed_part = raw_str.lstrip()
        orig_size = len(raw_str)

        while unprocessed_part:
            start_index = orig_size - len(unprocessed_part)
            next_lexem, unprocessed_part = Lexer._get_first_lexem(unprocessed_part,
                                                                  start_index)
            unprocessed_part = unprocessed_part.lstrip()
            lexem_list.append(next_lexem)

        logging.debug('Lexer: {} was lexed '\
                      'to {}'.format(raw_str,
                                     ','.join(map(lambda lex: lex.get_type().name, lexem_list))))
        return lexem_list

    @staticmethod
    def _get_first_lexem(raw_str, start_idx_in_original):
        """Get the first lexem of the string.

        Args:
            raw_str (str): the string to be anaylzed;
            start_idx_in_original (int): an index of this
                string in an original string
                (of which this string is a part).

        Invariant: string is not empty, it does not start with whitespace.
        """
        raw_len = len(raw_str)

        if raw_str[0] == '|':
            return (Lexem(LexemType.PIPE, '|', start_idx_in_original,
                          start_idx_in_original),
                    raw_str[1:])
        elif raw_str[0] in ('"', "'"):
            next_quote_idx = 1

            while next_quote_idx < raw_len:
                if raw_str[next_quote_idx] == raw_str[0]:
                    break
                next_quote_idx += 1

            if next_quote_idx == raw_len:
                raise LexException('A non-terminating quoted string starting '\
                                   'at position {}'.format(start_idx_in_original))

            return (Lexem(LexemType.QUOTED_STRING, raw_str[:next_quote_idx + 1],
                          start_idx_in_original,
                          start_idx_in_original + next_quote_idx),
                    raw_str[next_quote_idx + 1:])
        else:
            whitespace_or_quote_idx = 1

            while whitespace_or_quote_idx < raw_len:
                cur_char = raw_str[whitespace_or_quote_idx]
                if cur_char.isspace() or cur_char in ('"', "'"):
                    break
                whitespace_or_quote_idx += 1

            lexem_val = raw_str[:whitespace_or_quote_idx]
            rest_string = raw_str[whitespace_or_quote_idx:]

            if '=' in lexem_val:
                return (Lexem(LexemType.ASSIGNMENT, lexem_val,
                              start_idx_in_original,
                              start_idx_in_original + whitespace_or_quote_idx - 1),
                        rest_string)

            return (Lexem(LexemType.STRING, lexem_val,
                          start_idx_in_original,
                          start_idx_in_original + whitespace_or_quote_idx - 1),
                    rest_string)
