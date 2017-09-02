n = int(input())
s = input()

cnt = [0 for i in range(10)]

for c in s:
    cnt[int(c)] += 1

first_pos = 1
for i in range(1, 10):
    if cnt[i] > 0:
        first_pos = i
        break

cnt[first_pos] -= 1
res = ''.join(chr(ord('0') + i) * cnt[i] for i in range(10))
print(first_pos, end='')
print(res)
