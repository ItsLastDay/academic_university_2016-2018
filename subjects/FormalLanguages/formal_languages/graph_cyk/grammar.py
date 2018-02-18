from collections import defaultdict
import copy

import random
import string


def gen_random_name(prefix='', sz=7):
    return prefix + ''.join([random.choice(string.ascii_letters) for _ in range(sz)])


class Nonterminal:

    def __init__(self, name):
        self.name = name

    def __eq__(self, other):
        return isinstance(other, Nonterminal) and self.name == other.name

    def __hash__(self):
        return hash(self.name)

    def __lt__(self, other):
        return self.name < other.name

    def __repr__(self):
        return self.name
        

class Terminal:

    def __init__(self, content):
        self.content = content

    def __repr__(self):
        return self.content


class Eps:

    def __repr__(self):
        return 'eps'

    def __eq__(self, other):
        return isinstance(other, Eps)


class Grammar:

    def __init__(self, start, rules):
        self.start = start
        self.rules = rules

    @staticmethod
    def _parse_rules(rules):
        nonterm_to_alts = defaultdict(list)
        for line in rules:
            nterm, alts = map(lambda x: x.strip(), line.split('='))
            nterm = Nonterminal(nterm)

            for alt in alts.split('|'):
                alt = alt.strip()
                alt_list = []
                if alt == 'eps':
                    alt_list.append(Eps())
                else:
                    for inside_alt in alt.split(' '):
                        inside_alt = inside_alt.strip()
                        if inside_alt.startswith("'") and inside_alt.endswith("'"):
                            alt_list.append(Terminal(inside_alt))
                        else:
                            alt_list.append(Nonterminal(inside_alt))

                nonterm_to_alts[nterm].append(alt_list)

        return nonterm_to_alts
        

    @staticmethod
    def read_from_file(filename):
        with open(filename, 'r') as fl:
            lines = fl.read().strip().splitlines()
            start, rules = lines[0], lines[1:]
            return Grammar(Nonterminal(start), Grammar._parse_rules(rules))

    def to_string(self):
        res = str(self.start) + '\n'

        for nonterm_from, alts_to in sorted(self.rules.items()):
            if not alts_to:
                continue
            
            strings_alts = map(lambda x: ' '.join(map(str, x)), alts_to)
            alts_str = ' | '.join(strings_alts)
            res += '{} = {}\n'.format(nonterm_from, alts_str)

        return res

    def to_grammar_without_term_right(self):
        # A -> x 'a' y => A -> x B y ; B -> 'a'
        new_rules = defaultdict(list)

        added_term_to_nonterm = dict()
        for nonterm_from, alts_to in self.rules.items():
            new_rules_for_this_alt = []
            for alt in alts_to:
                if not alt or len(alt) == 1 or not any(map(lambda x: isinstance(x, Terminal), alt)):
                    new_rules_for_this_alt.append(alt)
                    continue
                for i in range(len(alt)):
                    if not isinstance(alt[i], Terminal):
                        continue
                    if alt[i] not in added_term_to_nonterm:
                        added_term_to_nonterm[alt[i]] = Nonterminal(alt[i].content.strip("'") + 'ADDED')

                subst = lambda x: x if isinstance(x, Nonterminal) else added_term_to_nonterm[x]
                new_rules_for_this_alt.append(list(map(subst, alt)))
                    
            new_rules[nonterm_from] = new_rules_for_this_alt

        for term, nonterm in added_term_to_nonterm.items():
            new_rules[nonterm] = [[term]]

        return Grammar(self.start, new_rules)


    def to_grammar_without_eps(self):
        can_reach_eps = []

        for nonterm_from, alts_to in self.rules.items():
            can_eps = False
            for alt in alts_to:
                if len(alt) == 1 and isinstance(alt[0], Eps):
                    can_eps = True
            if can_eps:
                can_reach_eps.append(nonterm_from)

        any_changes = True
        while any_changes:
            any_changes = False

            for nterm, alts in self.rules.items():
                if nterm in can_reach_eps:
                    continue

                for alt in alts:
                    if all(map(lambda x: x in can_reach_eps, alt)):
                        any_changes = True
                        can_reach_eps.append(nterm)

        new_rules = defaultdict(list)
        new_start = Nonterminal(self.start.name + 'NEW')
        new_rules[new_start].append([self.start])
        if self.start in can_reach_eps:
            new_rules[new_start].append([Eps()])

        for nterm, alts_to in self.rules.items():
            for alt in alts_to:
                if len(alt) == 1 and isinstance(alt[0], Eps):
                    continue
                num_bad = len(list(filter(lambda x: x in can_reach_eps, alt)))

                for rec in range(1 << num_bad):
                    new_lst = []
                    pos = 0
                    for sym in alt:
                        if sym in can_reach_eps:
                            if (1 << pos) & rec:
                                new_lst.append(sym)
                            pos += 1
                        else:
                            new_lst.append(sym)

                    if new_lst:
                        new_rules[nterm].append(new_lst)
            
        return Grammar(new_start, new_rules)


    def to_grammar_without_chain_rules(self):
        new_rules = copy.deepcopy(self.rules)

        any_changes = True
        while any_changes:
            any_changes = False

            for nterm, alts in new_rules.items():
                while [nterm] in alts:
                    new_rules[nterm].remove([nterm])

            fnd_from = None
            fnd_to = None
            for nterm, alts in new_rules.items():
                for alt in alts:
                    if len(alt) == 1 and isinstance(alt[0], Nonterminal):
                        fnd_from = nterm
                        fnd_to = alt[0]

            if fnd_from is None:
                continue

            new_rules[fnd_from].remove([fnd_to])

            any_changes = True
            for alt in new_rules[fnd_to]:
                new_rules[fnd_from].append(alt)

        return Grammar(self.start, new_rules)


    def to_grammar_without_unused_rules(self):
        reachable_from_start = set([self.start])
        queue = [self.start]
        while queue:
            cur = queue.pop(0)
            for alt in self.rules[cur]:
                for sym in alt:
                    if isinstance(sym, Nonterminal):
                        if sym in reachable_from_start:
                            continue
                        reachable_from_start.add(sym)
                        queue.append(sym)

        reachable_from_terminal = set()

        any_changes = True
        while any_changes:
            any_changes = False

            for nterm, alts in self.rules.items():
                for alt in alts:
                    if (len(alt) == 1 and isinstance(alt[0], (Eps, Terminal)) or (
                        all(map(lambda x: x in reachable_from_terminal, alt)))):
                        if nterm not in reachable_from_terminal:
                            reachable_from_terminal.add(nterm)
                            any_changes = True

        good_set = reachable_from_start.intersection(reachable_from_terminal)

        new_rules = defaultdict(list)
        for nterm, alts in self.rules.items():
            if nterm not in good_set:
                continue
            for alt in alts:
                if any(map(lambda x: not isinstance(x, (Eps, Terminal)) and x not in good_set, alt)):
                    continue
                new_rules[nterm].append(alt)

        return Grammar(self.start, new_rules)


    def to_grammar_without_long_rules(self):
        new_rules = defaultdict(list)

        for nterm, alts in self.rules.items():
            for alt in alts:
                if len(alt) <= 2:
                    new_rules[nterm].append(alt)
                else:
                    base = Nonterminal(gen_random_name())
                    new_rules[base].append(alt[-2:])
                    for i in range(len(alt) - 3, 0, -1):
                        new_base = Nonterminal(gen_random_name())
                        new_rules[new_base].append([alt[i], base])
                        base = new_base

                    new_rules[nterm].append([alt[0], base])

        return Grammar(self.start, new_rules)


    def to_chomsky_normalized(self):
        gr = self.to_grammar_without_term_right()
        gr = gr.to_grammar_without_eps()
        gr = gr.to_grammar_without_chain_rules()
        gr = gr.to_grammar_without_unused_rules()
        gr = gr.to_grammar_without_long_rules()
        return gr



if __name__ == '__main__':
    gr = Grammar.read_from_file('./test_arith.txt')
    print(gr.to_string())
    gr = gr.to_grammar_without_term_right()
    print('*****term right')
    print(gr.to_string())
    gr = gr.to_grammar_without_eps()
    print('*****w/o eps')
    print(gr.to_string())
    gr = gr.to_grammar_without_chain_rules()
    print('*****chain rules')
    print(gr.to_string())
    gr = gr.to_grammar_without_unused_rules()
    print('*****unused rules')
    print(gr.to_string())
    gr = gr.to_grammar_without_long_rules()
    print('*****long rules')
    print(gr.to_string())
