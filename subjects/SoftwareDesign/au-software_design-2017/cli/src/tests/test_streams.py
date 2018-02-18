import unittest
import os

from cli.streams import OutputStream, InputStream


class StreamsTest(unittest.TestCase):
    """Tests for ensuring I/O correctness of Streams.
    """

    def test_write_convert_read(self):
        out_stream = OutputStream()

        out_stream.write_line('xyz')
        out_stream.write('1')

        inp_stream = out_stream.to_input_stream()
        self.assertEqual(inp_stream.get_input(), 'xyz{}1'.format(os.linesep))
