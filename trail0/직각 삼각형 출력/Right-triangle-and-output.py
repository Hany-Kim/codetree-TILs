n = int(input())

for y in range(0, n):
    for x in range(0, (2*y)+1):
        print('*', end='')
    print()