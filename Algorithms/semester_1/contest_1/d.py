q, d, a = map(int, input().split())

f = 120 // q

if d * f <= a or d <= 7:
    print('Hideout')
else:
    print('Fear not')
