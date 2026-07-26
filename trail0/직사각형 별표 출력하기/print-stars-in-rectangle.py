n, m = map(int, input().split())

mmap = [['*'] * m for _ in range(n)]

for y in range(n):
    for x in range(m):
        print(mmap[y][x], end = ' ')
    print()