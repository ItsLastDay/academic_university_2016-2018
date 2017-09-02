import itertools

alph = 'acgt'

def get_circle():
    res = 'ANY = {}\n'.format(' | '.join(map(lambda s: "'{}'".format(s), alph)))
    res += 'TWO_ANY = ANY ANY\n'
    res += 'CIRCLE = TWO_ANY | ANY TWO_ANY | TWO_ANY TWO_ANY'
    return res


def stem(resulting_nterm, interim_res):
    pairs = ('ac', 'ca', 'gt', 'tg')

    inner = resulting_nterm + '_INNER'
    res = '{} = '.format(inner)
    for i, p in enumerate(pairs):
        res += " {} '{}' {} '{}'".format('|' if i > 0 else '',
                p[0], interim_res, p[1])

    res += '\n'
    res += '{} = '.format(resulting_nterm)
    for i, p in enumerate(pairs):
        res += " {} '{}' {} '{}'".format('|' if i > 0 else '',
                p[0], inner, p[1])
        res += " {} '{}' {} '{}'".format('|',
                p[0], resulting_nterm, p[1])

    return res


print('START')

print(get_circle())

print(stem('BRANCH', 'CIRCLE'))

print(stem('START', 'BRANCH BRANCH BRANCH'))
