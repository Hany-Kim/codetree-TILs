mat = [list(map(int, input().split())) for _ in range(4)]

for y in range(4):
    sum = 0
    for x in range(4):
        sum += mat[y][x]
    print(sum)