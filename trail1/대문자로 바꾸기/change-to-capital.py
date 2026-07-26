mat = [list(input().split()) for _ in range(5)]

for y in range(5):
    for x in range(3):
        mat[y][x] = mat[y][x].upper()
        print(mat[y][x], end=' ')
    print()