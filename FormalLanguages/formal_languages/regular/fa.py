import abc
import random
import string

import nfa


class FiniteAutomata(metaclass=abc.ABCMeta):
    """Base class for Finite Automata. Implements basic operations.

    Does not pretend to be optimal. Instead, aims for maximum reliability.
    """

    @abc.abstractmethod
    def check_self(self):
        return NotImplemented

    def __str__(self):
        str_representation = 'states = {}'.format(self.states)
        str_representation += ', initial_state = {}'.format(self.initial_state)
        str_representation += ', terminal_states = {}'.format(self.terminal_states)
        str_representation += ', transitions: {}'.format(self.transitions)
        str_representation += ', alphabet = {}'.format(self.alphabet)
        return 'AUTOMATON({})'.format(str_representation)

    def __init__(self,
                 states,
                 initial_state,
                 terminal_states,
                 transitions,
                 alphabet):
        """
        Args:
            states (set): a set of strings - automata state names. 
            initial_state (str): an initial state.
            terminal_states (set): a set of strings - terminal states names.
            transitions (dict): a mapping from <cur_state, letter> to
                list of states.
            alphabet (set): a string of possible characters.

        Example usage:
            FiniteAutomata({'q0', 'q1', 'q2'},
                           'q0',
                           {'q0', 'q1'},
                           dict([(('q0', 'x'), ['q1', 'q2']),
                                 (('q1', 'y'), ['q0', 'q1']),
                                 (('q2', 'x'), []),
                                 (('q2', 'y'), ['q0'])]),
                           'xyz')
        """
        self.states = states
        self.initial_state = initial_state
        assert self.initial_state in self.states

        self.terminal_states = terminal_states
        assert self.terminal_states.issubset(self.states)

        self.alphabet = alphabet

        self.transitions = transitions
        for key, val in self.transitions.items():
            st_from, letter = key

            assert st_from in self.states

            for to_state in val:
                assert to_state in self.states, (key, val)

        self.check_self()
        self.precalc()

    def to_dot(self, graph_name='automata'):
        old_name_to_new_name = dict()

        def name_to_dot(node_name):
            return '"{}"'.format(old_name_to_new_name.get(node_name, node_name))

        def letter_to_dot(letter):
            if letter == ' ':
                return '/s'
            if letter is nfa.NFA.EPS:
                return 'eps'
            return letter

        def dfs_rename(state, was):
            was.add(state)
            old_name_to_new_name[state] = str(len(was))

            for ch in (list(self.alphabet) + [nfa.NFA.EPS]):
                key = (state, ch)

                for next_state in self.transitions.get(key, []):
                    if next_state in was:
                        continue
                    dfs_rename(next_state, was)

        dfs_rename(self.initial_state, set())

        dot_representation = "digraph {} {}\n".format(graph_name, '{')
        dot_representation += 'rankdir=LR\n'
        dot_representation += 'node [shape = point, color=white, fontcolor=white]; start;\n'
        dot_representation += 'node [shape = doublecircle, color = black, fontcolor = black]; '
        dot_representation += ' '.join(map(name_to_dot, self.terminal_states)) + ';\n'
        dot_representation += 'node [shape = circle];'
        sorted_alph = ''.join(sorted(self.alphabet))
        dot_representation += 'start -> {} [ label = "{}" ];\n'.format(name_to_dot(self.initial_state), sorted_alph)
        for key, to_states in self.transitions.items():
            from_state, letter = key
            for to_state in to_states:
                dot_representation += '{} -> {} [ label = "{}" ];\n'.format(name_to_dot(from_state), 
                        name_to_dot(to_state), 
                        letter_to_dot(letter))
        dot_representation += '}'
        return dot_representation

    def write_dot_to_file(self, filename):
        dot_repr = self.to_dot()
        with open(filename, 'w') as fl:
            fl.write(dot_repr)

    def combine_names(self, names):
        """Create a new state name from an iterable of 
        consisting state names (for example, when doing minimization).
        """
        return ','.join(sorted(names))

    @staticmethod
    def gen_random_name(prefix='', sz=8):
        return prefix + ''.join([random.choice(string.ascii_letters) for _ in range(sz)])

    @abc.abstractmethod
    def accept_word(self, word):
        return NotImplemented

    def precalc(self):
        pass
