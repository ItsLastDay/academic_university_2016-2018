import sys
import os.path
import csv
import random
import string

from grammar import Grammar, Terminal, Nonterminal, Eps


def gen_random_name(prefix='', sz=8):
    return prefix + ''.join([random.choice(string.ascii_letters) for _ in range(sz)])


def cyk(gr, line, filename):
    print(gr.to_string())
    n = len(line)

    if n == 0:
        return [Eps()] in gr[gr.start]

    dp = [[set() for i in range(n)] for j in range(n)]
    prev = [[dict() for i in range(n)] for j in range(n)]

    for nterm, alts in gr.rules.items():
        for alt in alts:
            if len(alt) == 1 and isinstance(alt[0], Terminal):
                for i in range(n):
                    if line[i] == alt[0].content.strip("'"):
                        dp[i][i].add(nterm)

    for sz in range(2, n + 1):
        for start_idx in range(n):
            if start_idx + sz - 1 >= n:
                break

            end_idx = start_idx + sz - 1
            for nx_idx in range(start_idx, end_idx): # end not included
                for first_part in dp[start_idx][nx_idx]:
                    for second_part in dp[nx_idx + 1][end_idx]:
                        for nterm in gr.rules.keys():
                            if [first_part, second_part] in gr.rules[nterm]:
                                prev[start_idx][end_idx][nterm] = (first_part, second_part, nx_idx)
                                dp[start_idx][end_idx].add(nterm)


    with open('{}cyk_output.csv'.format(filename), 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        for i in range(n):
            writer.writerow(dp[i])

    resulting_graph = 'digraph cyk_output {}\nrankdir=TB;\n'.format('{')
    def dfs(cur_rule, i, j, parent_name=None):
        nonlocal resulting_graph
        cur_name = gen_random_name()
        cur_label = cur_rule.name
        resulting_graph += 'node [ label = "{}" ]; {} ;\n'.format(cur_label, cur_name)

        if parent_name is not None:
            resulting_graph += '{} -> {} ;\n'.format(parent_name, cur_name)

        if i == j:
            dummy_node = gen_random_name()
            resulting_graph += 'node [ label = "{}" ]; {} ;\n'.format(line[i], dummy_node)
            resulting_graph += '{} -> {} ;\n'.format(cur_name, dummy_node)
        else:
            left, right, sp_idx = prev[i][j][cur_rule]
            dfs(left, i, sp_idx, cur_name)
            dfs(right, sp_idx + 1, j, cur_name)
            

    if gr.start in dp[0][n - 1]:
        dfs(gr.start, 0, n - 1)
        resulting_graph += '}'

        with open('{}cyk_parse_tree.dot'.format(filename), 'w') as out:
            out.write(resulting_graph)

    return gr.start in dp[0][n - 1]


def main(argv):
    filename_grammar = argv[1]
    filename_sample = argv[2]

    gr = Grammar.read_from_file(filename_grammar).to_chomsky_normalized()

    print(cyk(gr, open(filename_sample, 'r').read().strip(),
            os.path.basename(filename_sample))) 

if __name__ == '__main__':
    main(sys.argv)
