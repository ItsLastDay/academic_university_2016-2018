# Formal languages

Implemented the following things:
- working with regular languages: [regular expressions](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/regexp.py) (RE),
[deterministic finite automata](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/dfa.py) (DFA),
[nondetermenistic finite automata](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/nfa.py) (NFA).  
Algorithms: [minimization of DFA](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/dfa.py#L130-L205),
[transforming RE to NFA](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/regexp.py#L36-L38),
[transforming NFA to DFA](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/regular/nfa.py#L104-L141);
- [Cocke–Younger–Kasami algorithm](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/cyk/main.py) for parsing text using context-free grammars;
- [Variation](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/FormalLanguages/formal_languages/graph_cyk/main.py) 
of Cocke–Younger–Kasami algorithm for parsing graphs (as opposed to linear text) with letters on edges using context-free grammars;
- [Parser](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/FormalLanguages/formal_languages/parser) and
[lexer](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/FormalLanguages/formal_languages/lexer) for toy language named `L` using ANTLR4.
