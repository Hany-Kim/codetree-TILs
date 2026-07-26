n = int(input())

for y in range(1, n+1):
    for x in range(1, n+1):
        if x == n:
            print(f'{y} * {x} = {y*x}', end='')
        else :
            print(f'{y} * {x} = {y*x}', end=', ')

    print()