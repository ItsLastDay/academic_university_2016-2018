from dfa import DFA
from nfa import NFA
from fa import FiniteAutomata
from regexp import EmptyRE, EpsilonRE, LetterRE, KleeneRE, ParensRE, UnionRE, ConcatRE 


EPS = NFA.EPS


def test_minimize_hw2_1():
    # Automaton is already complete.
    # Should reduce to a 3-state one.
    automaton = DFA({'A', 'B', 'D', 'C', 'E'},
                    'A',
                    {'D', 'C'},
                    dict([
                        (('A', '0'), ['B']),
                        (('A', '1'), ['C']),
                        (('B', '0'), ['D']),
                        (('B', '1'), ['E']),
                        (('D', '0'), ['B']),
                        (('D', '1'), ['C']),
                        (('E', '0'), ['D']),
                        (('E', '1'), ['E']),
                        (('C', '0'), ['B']),
                        (('C', '1'), ['C']),
                        ]),
                    '01')

    automaton.write_dot_to_file('hw2_1_orig.dot')

    minimized = automaton.to_minimal_dfa()
    minimized.write_dot_to_file('hw2_1_min.dot')

def test_minimize_noncomplete():
    # Minimizing non-complete DFA must be careful.
    # This automaton is already minimal.
    automaton = DFA({'A', 'BCD', 'EF', 'CD', 'E', 'F'},
                    'A',
                    {'BCD', 'CD', 'EF', 'E'},
                    dict([
                        (('A', 'a'), ['BCD']),
                        (('BCD', 'b'), ['EF']),
                        (('EF', 'a'), ['CD']),
                        (('EF', 'b'), ['E']),
                        (('CD', 'b'), ['F']),
                        (('F', 'a'), ['CD']),
                        (('E', 'b'), ['E'])
                        ]),
                    'ab')

    automaton.write_dot_to_file('noncomplete_orig.dot')

    complete = automaton.to_complete_dfa()
    complete.write_dot_to_file('noncomplete_complete.dot')

    minimized = automaton.to_minimal_dfa()
    minimized.write_dot_to_file('noncomplete_min.dot')


def test_minimize_has_unreachable():
    # Automaton has non-producing states and unreachable states.
    automaton = DFA({'A', 'B', 'H', 'C', 'F', 'E', 'D', 'G', 'X'},
            'A',
            {'G', 'F'},
            dict([
                (('A', '0'), ['H']),
                (('A', '1'), ['B']),
                (('B', '0'), ['H']),
                (('B', '1'), ['A']),
                (('H', '0'), ['C']),
                (('H', '1'), ['C']),
                (('C', '0'), ['E']),
                (('C', '1'), ['F']),
                (('E', '0'), ['F']),
                (('E', '1'), ['G']),
                (('D', '0'), ['E']),
                (('D', '1'), ['F']),
                (('G', '0'), ['G']),
                (('G', '1'), ['F']),
                (('F', '0'), ['F']),
                (('F', '1'), ['F']),
                (('X', '1'), ['X']),
                ]),
            '01')
    automaton.write_dot_to_file('unreachable_orig.dot')
    minimized = automaton.to_minimal_dfa()
    minimized.write_dot_to_file('unreachable_min.dot')


def test_nfa_eps_jumps():
    automaton = NFA({'A', 'B', 'C', 'D', 'E', 'X'},
            'A',
            {'D'},
            dict([
                (('A', EPS), ['B', 'X']),
                (('A', 'a'), ['C', 'D']),
                (('B', EPS), ['C']),
                (('C', 'a'), ['D']),
                (('D', EPS), ['E'])
                ]),
            'a')
    automaton.write_dot_to_file('nfa_eps_jumps.dot')
    
    autom_no_eps = automaton.to_nfa_without_eps_jumps()
    autom_no_eps.write_dot_to_file('nfa_eps_jumps_erased.dot')


def test_nfa_exponential():
    # An NFA for the language "the 3rd letter from the
    # end is a". It must have 2^3 nodes.
    automaton = NFA({'A', 'B', 'C', 'D'},
            'A',
            {'D'},
            dict([
                (('A', 'a'), ['A', 'B']),
                (('A', 'b'), ['A']),
                (('B', 'a'), ['C']),
                (('B', 'b'), ['C']),
                (('C', 'a'), ['D']),
                (('C', 'b'), ['D']),
                ]),
            'ab')
    automaton.write_dot_to_file('exponential_orig.dot')
    assert automaton.accept_word('aaaaaa')
    assert not automaton.accept_word('bbbabbb')
    assert automaton.accept_word('bbbbabb')
    assert not automaton.accept_word('')
    
    determinized = automaton.to_dfa()
    determinized.write_dot_to_file('exponential_determinized.dot')
    assert determinized.accept_word('aaaaaa')
    assert not determinized.accept_word('bbbabbb')
    assert determinized.accept_word('bbbbabb')
    assert not determinized.accept_word('')


def test_regexp_even_ones():
    # Regexp "(0|11)*
    exp = KleeneRE(ParensRE(UnionRE(LetterRE('0'), 
                                    ConcatRE(LetterRE('1'), LetterRE('1')))))
    derived_nfa = exp.to_nfa()
    derived_nfa.write_dot_to_file('even_ones_nfa.dot')

    assert derived_nfa.accept_word('')
    assert derived_nfa.accept_word('0000')
    assert not derived_nfa.accept_word('00001')
    assert derived_nfa.accept_word('11')
    assert not derived_nfa.accept_word('111')
    assert not derived_nfa.accept_word('11110001')
    assert derived_nfa.accept_word('1111000110111111')


def test_regexp_numbers():
    # Regular expression for numbers with non-leading zeroes.
    # d ::= 1|2|3|4|5|6|7|8|9
    # num ::= 0|d(0|d)*
    digit = LetterRE('1')
    for i in range(2, 10):
        digit = UnionRE(digit, LetterRE(str(i)))

    number = UnionRE(LetterRE('0'),
                     ConcatRE(digit, 
                         KleeneRE(ParensRE(UnionRE(LetterRE('0'),
                                                   digit)))))

    number_nfa = number.to_nfa()                         
    number_dfa = number_nfa.to_dfa()

    assert number_dfa.accept_word('0')
    assert number_dfa.accept_word('1')
    assert number_dfa.accept_word('1239576598763456')
    assert number_dfa.accept_word('102')
    assert number_dfa.accept_word('800000000000000009')
    assert not number_dfa.accept_word('')
    assert not number_dfa.accept_word('qqq')
    assert not number_dfa.accept_word('0123')
    assert not number_dfa.accept_word('1e5')
    assert not number_dfa.accept_word('1239576598763456.5')


if __name__ == '__main__':
    # Manual tests. Need to run an verify by hand.

    # DFA TESTS:
    test_minimize_hw2_1()
    test_minimize_noncomplete()
    test_minimize_has_unreachable()


    # NFA TESTS:
    test_nfa_eps_jumps()
    test_nfa_exponential()


    # REGEXP TESTS:
    test_regexp_even_ones()
    test_regexp_numbers()
