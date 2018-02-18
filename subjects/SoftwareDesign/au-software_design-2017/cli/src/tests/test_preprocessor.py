import unittest

from cli.preprocessor import Preprocessor
from cli.environment import Environment


class PreprocessortTest(unittest.TestCase):
    """Tests on preprocessing: whether string variables are exapnded OK.
    """
    
    def setUp(self):
        env = Environment()
        env.set_var('x', '1')
        env.set_var('long_name', 'qwe')
        env.set_var('y', '59')
        self.env = env

        self.preproc = lambda s: Preprocessor.substitute_environment_variables(s, self.env)

    def test_var_endquote(self):
        processed_string = self.preproc('echo "123$x"')
        self.assertEqual(processed_string, 'echo "1231"')

    def test_only_var(self):
        processed_string = self.preproc('$x')
        self.assertEqual(processed_string, '1')

    def test_var_space_eol(self):
        processed_string = self.preproc('echo "123$x "')
        self.assertEqual(processed_string, 'echo "1231 "')

    def test_nonexistent_var_quote(self):
        processed_string = self.preproc('cat "$nonexc "')
        self.assertEqual(processed_string, 'cat " "')

    def test_var_quote_space(self):
        processed_string = self.preproc('echo "123$x dfg"')
        self.assertEqual(processed_string, 'echo "1231 dfg"')

    def test_varname_concat_of_two(self):
        # `$xy` must not be treated as `$x$y`.
        processed_string = self.preproc('$xy')
        self.assertEqual(processed_string, '')
    
    def test_var_before_single_quote(self):
        processed_string = self.preproc("echo $long_name'123'")
        self.assertEqual(processed_string, "echo qwe'123'")

    def test_var_before_double_quote(self): 
        processed_string = self.preproc("echo $long_name\"123\"")
        self.assertEqual(processed_string, "echo qwe\"123\"")

    def test_var_unquoted(self):
        processed_string = self.preproc("echo $y $x")
        self.assertEqual(processed_string, "echo 59 1")

    def test_vars_together(self):
        processed_string = self.preproc("echo $long_name$x")
        self.assertEqual(processed_string, "echo qwe1")

    def test_vars_together_dquoted(self):
        processed_string = self.preproc("echo \"$y$x\"")
        self.assertEqual(processed_string, "echo \"591\"")

    def test_var_in_single_quote(self):
        processed_string = self.preproc("echo '$x''$y'")
        self.assertEqual(processed_string, "echo '$x''$y'")

    def test_stray_dollar(self):
        processed_string = self.preproc("echo $ \"$\" '$' \"$ \"")
        self.assertEqual(processed_string, "echo $ \"$\" '$' \"$ \"")

    def test_double_dollar(self):
        processed_string = self.preproc("echo $$x \"$$y\"")
        self.assertEqual(processed_string, "echo $1 \"$59\"")

    def test_quote_after_dollar(self):
        processed_string = self.preproc("echo $'' $\"\"")
        self.assertEqual(processed_string, "echo $'' $\"\"")
