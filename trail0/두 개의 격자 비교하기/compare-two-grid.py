n, m = map(int, input().split())

mat1 = [list(map(int, input().split())) for _ in range(n)]
mat2 = [list(map(int, input().split())) for _ in range(n)]

res = [[0] * m for _ in range(n)]

for y in range(n):
    for x in range(m):
        if mat1[y][x] == mat2[y][x]:
            res[y][x] = 0
        else :
            res[y][x] = 1

        print(res[y][x], end=' ')
    print()