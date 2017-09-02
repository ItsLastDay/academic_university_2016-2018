from koltsov_prof import prof



@prof('ab.prof')
def ab(a, b):
    return a ** b


for a in range(10000, 10007):
    for b in range(10000, 10008):
        print(a, b, ab(a,b))
