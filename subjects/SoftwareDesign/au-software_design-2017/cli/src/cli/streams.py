"""Abstractions of command input and output.

Every command accepts some input and results
in some output. This module contains abstractions
on this ideas.
"""
from io import StringIO
import os


class _BaseStream:
    """A common implementation detail for Input- and Output-Stream.

    Both streams are implemented as a wrapper above io.StringIO.
    So, technically, they are reading and writing from a in-memory string.
    """

    def __init__(self):
        """Construct a Stream (both Input and Output)"""
        self._io_obj = StringIO()


class InputStream(_BaseStream):
    """An abstraction of command's input.

    A command can read from InputStream.
    """

    def get_input(self):
        """Read the whole input (as a string)"""
        return self._io_obj.getvalue()


class OutputStream(_BaseStream):
    """An abstraction of command's output.

    A command can write into OutputStream.
    """

    def write(self, string):
        """Write a string to output stream"""
        self._io_obj.write(string)

    def write_line(self, string):
        """Write a newline-trailed string to output stream"""
        self.write(string)
        self.write(os.linesep)

    def to_input_stream(self):
        """Convert this OutputStream to an InputStream.

        As a result of pipe, e.g. "echo 123 | wc", a command's
        output becomes another command's input.
        """
        inp_stream = InputStream()
        inp_stream._io_obj.write(self._io_obj.getvalue())
        return inp_stream
