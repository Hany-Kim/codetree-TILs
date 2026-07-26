mmap1 = []
mmap2 = []

for i in range(3):
    mlist = list(map(int, input().split()))
    mmap1.append(mlist)

input()

for i in range(3):
    mlist = list(map(int, input().split()))
    mmap2.append(mlist)

for y in range(3):
    for x in range(3):
        res = mmap1[y][x] * mmap2[y][x]

        print(res, end = ' ')
    print()