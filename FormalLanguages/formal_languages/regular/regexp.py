from nfa import NFA
from fa import FiniteAutomata

import abc
import copy


gen_random_name = lambda prefix: FiniteAutomata.gen_random_name(prefix, 5)


class AcademicRegularExpression(metaclass=abc.ABCMeta):
    """Base class for Academic Regular Expressions over some alphabet.

    The are 7 types of such expressions:
        1. Empty set        language is Empty
        2. epsilon          language is {eps}
        3. letter from alphabet     language is that one letter
        4. parenthised regexp       language is the same as inside parenthesis
        5. Kleene star(r)          language is {w | w=w1w2...w_n, n >= 0, w_i \in r
        6. Concatenation(r1, r2)    language is {w | w=uv, u \in r1, v \in r2}
        7. Union(r1, r2)        language is r1 + r2

    Parsing them is cumbersome, because epsilon and emptyset are hard to express.
    So I have several derived classes - one for each type of regexp.

    You can transform an ARE to NFA. Each obtained NFA must
    have
        - exactly one terminal state
        - no transitions to initial state
        - no transitions from terminal state
        - terminal and initial states are distinct.

    The alphabet is inferred from all sub-regexps.
    """

    @abc.abstractmethod
    def to_nfa(self):
        pass


class EmptyRE(AcademicRegularExpression):
    def to_nfa(self):
        start = gen_random_name('start_')
        end = gen_random_name('end_')
        return NFA({start, end},
            start,
            {end},
            dict([]),
            '')


class EpsilonRE(AcademicRegularExpression):
    def to_nfa(self):
        start = gen_random_name('start_')
        end = gen_random_name('end_')
        return NFA({start, end},
            start,
            {end},
            dict([
                ((start, NFA.EPS), [end])
                ]),
            '')


class LetterRE(AcademicRegularExpression):
    def __init__(self, letter):
        self.letter = letter
        assert len(self.letter) == 1


    def to_nfa(self):
        start = gen_random_name('start_')
        end = gen_random_name('end_')
        return NFA({start, end},
            start,
            {end},
            dict([
                ((start, self.letter), [end])
                ]),
            self.letter)


class ParensRE(AcademicRegularExpression):
    def __init__(self, regexp):
        self.regexp = regexp

    def to_nfa(self):
        return self.regexp.to_nfa()


class KleeneRE(AcademicRegularExpression):
    def __init__(self, regexp):
        self.regexp = regexp

    def to_nfa(self):
        start = gen_random_name('start_')
        end = gen_random_name('end_')

        inner_nfa = self.regexp.to_nfa()

        transitions = inner_nfa.transitions
        initial_state = inner_nfa.initial_state
        terminal_state = list(inner_nfa.terminal_states)[0]
        alphabet = inner_nfa.alphabet
        states = inner_nfa.states
        
        new_transitions = copy.copy(transitions)
        new_transitions[(start, NFA.EPS)] = [initial_state, end]
        new_transitions[(terminal_state, NFA.EPS)] = [initial_state, end]

        return NFA(states.union({start, end}),
                start,
                {end},
                new_transitions,
                alphabet)


class ConcatRE(AcademicRegularExpression):
    def __init__(self, reg_left, reg_right):
        self.reg_left = reg_left
        self.reg_right = reg_right


    def to_nfa(self):
        nfa_left = self.reg_left.to_nfa()
        nfa_right = self.reg_right.to_nfa()

        new_transitions = dict()
        new_transitions.update(nfa_left.transitions)
        new_transitions.update(nfa_right.transitions)
        new_transitions[(list(nfa_left.terminal_states)[0],
            NFA.EPS)] = [nfa_right.initial_state]

        new_alphabet = ''.join(set(nfa_left.alphabet).union(
                set(nfa_right.alphabet)))

        return NFA(nfa_left.states.union(nfa_right.states),
                nfa_left.initial_state,
                nfa_right.terminal_states,
                new_transitions,
                new_alphabet)


class UnionRE(AcademicRegularExpression):
    def __init__(self, reg_left, reg_right):
        self.reg_left = reg_left
        self.reg_right = reg_right

    def to_nfa(self):
        start = gen_random_name('start_')
        end = gen_random_name('end_')

        nfa_left = self.reg_left.to_nfa()
        nfa_right = self.reg_right.to_nfa()

        new_transitions = dict()
        new_transitions.update(nfa_left.transitions)
        new_transitions.update(nfa_right.transitions)
        new_transitions[(start, NFA.EPS)] = [nfa_left.initial_state,
                nfa_right.initial_state]
        new_transitions[(list(nfa_left.terminal_states)[0], NFA.EPS)] = [end]
        new_transitions[(list(nfa_right.terminal_states)[0], NFA.EPS)] = [end]

        new_alphabet = ''.join(set(nfa_left.alphabet).union( 
                set(nfa_right.alphabet)))

        return NFA(nfa_left.states.union(nfa_right.states).union({start, end}),
                start,
                {end},
                new_transitions,
                new_alphabet)
