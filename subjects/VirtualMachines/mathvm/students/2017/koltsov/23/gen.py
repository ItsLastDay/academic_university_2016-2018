import re

with open('bytecode_translator_impl.cpp', 'r') as inp:
    cur = inp.read()

    for x in sorted(set(re.findall('BC_\w+', cur))):
        print('case {}: break;'.format(x))

