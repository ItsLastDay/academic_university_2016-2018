import sys
import os.path
import csv
import random
import string
import multiprocessing
import sys
from collections import defaultdict

from grammar import Grammar, Terminal, Nonterminal, Eps

ALPHABET = 'acgt'


def gen_random_name(prefix='', sz=8):
    return prefix + ''.join([random.choice(string.ascii_letters) for _ in range(sz)])

def gen_random_graph():
    # Returns: list of vertices, list of edges

    num_vertices = random.randint(50, 55)
    num_edges = random.randint(1000, 1200)

    all_vertices = []
    for i in range(num_vertices):
        all_vertices.append(gen_random_name())

    all_edges = []
    for i in range(num_edges):
        vert1 = random.choice(all_vertices)
        vert2 = random.choice(all_vertices)
        edge_label = random.choice(ALPHABET)

        all_edges.append((vert1, vert2, edge_label))

    return all_vertices, all_edges


def cyk_graph(gram, graph):
    print('Normalized grammar:')
    print(gram.to_string())
    vertices, edges = graph
    print('Graph has {} vertices and {} edges'.format(len(vertices), len(edges)))
    print('Grammar has {} nonterminals'.format(len(gram.rules.keys())))

    valid_triples = []

    if [Eps()] in gram.rules[gram.start]:
        for v in vertices:
            valid_triples.append((v, v, gram.start))

    for e in edges:
        fr, to, label = e

        for nterm, alts in gram.rules.items():
            for alt in alts:
                if len(alt) == 1 and isinstance(alt[0], Terminal) and alt[0].content.strip("'") == label:
                    valid_triples.append((fr, to, nterm))


    two_nterms_to_nterm = defaultdict(list)
    for nterm, alts in gram.rules.items():
        for alt in alts:
            if len(alt) != 2:
                continue
            if not (isinstance(alt[0], Nonterminal) and isinstance(alt[1], Nonterminal)):
                continue
            two_nterms_to_nterm[tuple(alt)].append(nterm)


    set_valid = set(valid_triples)
    del valid_triples

    any_changes = True
    while any_changes:
        any_changes = False

        set_added = set()

        triples_starting_by = defaultdict(list)
        for tr in set_valid:
            fr, _, _ = tr
            triples_starting_by[fr].append(tr)


        args = [(first_triple, triples_starting_by, set_added, set_valid) for first_triple in set_valid]
        for first_triple in set_valid:
            fr_first, to_first, nterm_first = first_triple

            for second_triple in triples_starting_by[to_first]:
                fr_second, to_second, nterm_second = second_triple

                for nterm in two_nterms_to_nterm[(nterm_first, nterm_second)]:
                    new_triple = (fr_first, to_second, nterm)
                    if new_triple in set_valid or new_triple in set_added:
                        continue
                    any_changes = True

                    set_added.add(new_triple)

        print('Current number of valid triples:', len(set_valid), 
                'number of added triples on this iteration:', len(set_added))
                
        set_valid.update(set_added)
        sys.stdout.flush()

    
    valid_triples_from_start = list(filter(lambda trip: trip[2] == gram.start, set_valid))
    return valid_triples_from_start


def print_graph(graph):
    # Write graph to 'output_graph.dot'.
    graph_dot = 'digraph cyk_output {}\nrankdir=TB;\n'.format('{')

    vertices, edges = graph

    for cur_name in vertices:
        graph_dot += 'node [ label = "{}" ]; {} ;\n'.format(cur_name, cur_name)

    for e in edges:
        fr, to, label = e
        graph_dot += '{} -> {} [ label = "{}" ] ;\n'.format(fr, to, label)


    graph_dot += '}'

    with open('output_graph.dot', 'w') as out:
        out.write(graph_dot)


def main(argv):
    random.seed(42) # Deterministic random.

    filename_grammar = './trna_grammar.txt'

    gram = Grammar.read_from_file(filename_grammar).to_chomsky_normalized()
    graph = gen_random_graph()

    print_graph(graph)

    triples = cyk_graph(gram, graph)

    print('\n{} pairs of vertices that have matching path:'.format(len(triples)))
    for tr in triples:
        fr, to, nterm = tr
        print(fr, '->', to)

if __name__ == '__main__':
    main(sys.argv)
