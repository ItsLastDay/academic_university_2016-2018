from regexp import EmptyRE, EpsilonRE, LetterRE, KleeneRE, ParensRE, UnionRE, ConcatRE 


def re_identifiers():
    """Implement and test a regexp for C-style identifiers.

    letter ::= a|b|...|z|A|B|...|Z
    digit ::= 0|1|...|9

    any_char ::= letter | digit | _
    identifier ::= (letter | _) (any_char)*
    """
    letter = LetterRE('a')
    for ch in range(1, 26):
        low = chr(ord('a') + ch)
        letter = UnionRE(letter, LetterRE(low))

    for ch in range(26):
        high = chr(ord('A') + ch)
        letter = UnionRE(letter, LetterRE(high))
    
    digit = LetterRE('0')
    for i in range(1, 10):
        digit = UnionRE(digit, LetterRE(str(i)))

    any_char = UnionRE(letter, UnionRE(digit, LetterRE('_')))
    identifier = ConcatRE(UnionRE(letter, LetterRE('_')),
                          KleeneRE(any_char))

    identifier_nfa = identifier.to_nfa()
    identifier_nfa.write_dot_to_file('identifier_nfa.dot')

    identifier_nfa_no_eps = identifier_nfa.to_nfa_without_eps_jumps()
    identifier_nfa_no_eps.write_dot_to_file('identifier_nfa_no_eps.dot')

    identifier_dfa = identifier_nfa_no_eps.to_dfa()
    identifier_dfa.write_dot_to_file('identifier_dfa.dot')

    identifier_dfa_min = identifier_dfa.to_minimal_dfa()
    identifier_dfa_min.write_dot_to_file('identifier_dfa_min.dot')

    assert identifier_dfa_min.accept_word('a')
    assert identifier_dfa_min.accept_word('_a')
    assert identifier_dfa_min.accept_word('A')
    assert identifier_dfa_min.accept_word('a_b')
    assert identifier_dfa_min.accept_word('iDeNt')
    assert identifier_dfa_min.accept_word('_i_D_')
    assert identifier_dfa_min.accept_word('__agent007__')
    assert identifier_dfa_min.accept_word('_13')
    assert identifier_dfa_min.accept_word('e2_e4')

    assert not identifier_dfa_min.accept_word('')
    assert not identifier_dfa_min.accept_word('1')
    assert not identifier_dfa_min.accept_word('1st')
    assert not identifier_dfa_min.accept_word('!myVar!')


def re_rational_numbers():
    """Implement and test a regexp for rational numbers.

    d ::= 1|2|3|...|9
    d0 ::= 0|d
    z ::= +|-|EPS
    x? ::= EPS|x
    num ::= d(d0)*|0
    num0 ::= (d0)(d0)*
    rational ::= -?(num)(.(num0)(ez(num))?)?
            | -?(num)(ez(num))?
            | -?.(num0)(ez(num))?
    """
    digit = LetterRE('1')
    for i in range(2, 10):
        digit = UnionRE(digit, LetterRE(str(i)))
    digit_0 = UnionRE(LetterRE('0'), digit)

    z = UnionRE(LetterRE('+'), UnionRE(LetterRE('-'), EpsilonRE()))
    opt = lambda reg: UnionRE(EpsilonRE(), reg)
    num = UnionRE(ConcatRE(digit, KleeneRE(digit_0)), LetterRE('0'))
    num_0 = ConcatRE(digit_0, KleeneRE(digit_0))

    minus = opt(LetterRE('-'))
    eznum = opt(ConcatRE(LetterRE('e'), ConcatRE(z, num)))
    rational = UnionRE(
            ConcatRE(minus, ConcatRE(num, opt(ConcatRE(LetterRE('.'), ConcatRE(num_0, eznum))))),
            UnionRE(
                ConcatRE(minus, ConcatRE(num, eznum)),
                ConcatRE(minus, ConcatRE(LetterRE('.'), ConcatRE(num_0, eznum)))
                ))

    rational_nfa = rational.to_nfa()
    rational_nfa.write_dot_to_file('rational_nfa.dot')

    rational_nfa_no_eps = rational_nfa.to_nfa_without_eps_jumps()
    rational_nfa_no_eps.write_dot_to_file('rational_nfa_no_eps.dot')

    rational_dfa = rational_nfa_no_eps.to_dfa()
    rational_dfa.write_dot_to_file('rational_dfa.dot')

    min_dfa = rational_dfa.to_minimal_dfa()
    min_dfa.write_dot_to_file('rational_dfa_min.dot')

    assert min_dfa.accept_word('-.2')
    assert min_dfa.accept_word('123e5')
    assert min_dfa.accept_word('13')
    assert min_dfa.accept_word('.00')
    assert min_dfa.accept_word('0')
    assert min_dfa.accept_word('-0')
    assert min_dfa.accept_word('10')
    assert min_dfa.accept_word('0.0')
    assert min_dfa.accept_word('0.1e0')
    assert min_dfa.accept_word('0.10010e+1')
    assert min_dfa.accept_word('10e-123')
    assert min_dfa.accept_word('.1')

    assert not min_dfa.accept_word('01')
    assert not min_dfa.accept_word('+.2')
    assert not min_dfa.accept_word('e')
    assert not min_dfa.accept_word('e.')
    assert not min_dfa.accept_word('.')
    assert not min_dfa.accept_word('')
    assert not min_dfa.accept_word('-.-')
    assert not min_dfa.accept_word('-.e')


def re_intlist():
    """Implement and test a regexp for lists of integers.

    space ::= (\s)*
    d ::= 1|2|3|...|9
    d0 ::= 0|d
    num ::= (EPS|-)d(d0)*|0

    list ::= [(space)*] | [((space)(num)(space);)*(space)(num)(space)]
    """
    space = KleeneRE(LetterRE(' '))

    digit = LetterRE('1')
    for i in range(2, 10):
        digit = UnionRE(digit, LetterRE(str(i)))
    digit_0 = UnionRE(LetterRE('0'), digit)

    num = ConcatRE(UnionRE(EpsilonRE(), LetterRE('-')), UnionRE(ConcatRE(digit, KleeneRE(digit_0)), LetterRE('0')))

    lst = UnionRE(
            ConcatRE(LetterRE('['), ConcatRE(space, LetterRE(']'))),
            ConcatRE(LetterRE('['),
                ConcatRE(KleeneRE(ConcatRE(space, ConcatRE(num, 
                    ConcatRE(space, LetterRE(';'))))),
                        ConcatRE(space, ConcatRE(num, ConcatRE(space,
                            LetterRE(']')))))))

    lst_nfa = lst.to_nfa()
    lst_nfa.write_dot_to_file('intlist_nfa.dot')

    lst_nfa_no_eps = lst_nfa.to_nfa_without_eps_jumps()
    lst_nfa_no_eps.write_dot_to_file('inlist_nfa_no_eps.dot')

    lst_dfa = lst_nfa_no_eps.to_dfa()
    lst_dfa.write_dot_to_file('inlist_dfa.dot')

    lst_dfa_min = lst_dfa.to_minimal_dfa()
    lst_dfa_min.write_dot_to_file('inlist_dfa_min.dot')

    assert lst_dfa_min.accept_word('[]')
    assert lst_dfa_min.accept_word('[1]')
    assert lst_dfa_min.accept_word('[1;1;2;3;5;8]')
    assert lst_dfa_min.accept_word('[   ]')
    assert lst_dfa_min.accept_word('[ 4; 8;   15; 16   ; 23; 42]')
    assert lst_dfa_min.accept_word('[ -500 ;              15; -3]')

    assert not lst_dfa_min.accept_word('][')
    assert not lst_dfa_min.accept_word('[[]]')
    assert not lst_dfa_min.accept_word('[1;2;3')
    assert not lst_dfa_min.accept_word('[a]')
    assert not lst_dfa_min.accept_word('[1,2,3]')
    assert not lst_dfa_min.accept_word('[1;2;]')
    assert not lst_dfa_min.accept_word('[1; 23  4; 5]')


if __name__ == '__main__':
    re_identifiers()
    re_rational_numbers()
    re_intlist()
