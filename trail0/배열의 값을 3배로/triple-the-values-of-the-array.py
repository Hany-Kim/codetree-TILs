mmap = []

for i in range(3):
    mlist = list(map(int, input().split()))
    mmap.append(mlist)

for y in range(3):
    for x in range(3):
        mmap[y][x] *= 3
        print(mmap[y][x], end=' ')
    print()