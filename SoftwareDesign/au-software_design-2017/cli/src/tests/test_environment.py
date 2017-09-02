import unittest

import os
import os.path

from cli.environment import Environment


class EnvironmentTest(unittest.TestCase):
    """Tests for the environment: put var, extract var, change dir.
    """

    def setUp(self):
        self.env = Environment()

    def test_reproducible_put(self):
        self.env.set_var('x', '123')
        self.assertEqual(self.env.get_var('x'), '123')

    def test_get_nonexistant(self):
        self.assertEqual(self.env.get_var('nonexistant'), '')

    def test_assign_twice(self):
        self.env.set_var('x', '123')
        self.env.set_var('x', 'qwe')
        self.assertEqual(self.env.get_var('x'), 'qwe')

    def test_assign_new_abspath(self):
        self.env.set_cwd(os.getcwd())
        self.assertEqual(self.env.get_cwd(), os.getcwd())

    def test_assign_new_relpath(self):
        cur_path = os.getcwd()
        new_path = os.path.join(cur_path, os.pardir)
        self.env.set_cwd(os.pardir)
        self.assertEqual(self.env.get_cwd(), new_path)
