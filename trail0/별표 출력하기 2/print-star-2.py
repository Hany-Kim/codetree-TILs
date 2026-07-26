n = int(input())

for y in range(n, 0, -1):
    for x in range(y):
        print('*', end=' ')
    print()