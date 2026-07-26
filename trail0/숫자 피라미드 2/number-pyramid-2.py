n = int(input())

idx = 1;
for y in range(1, n+1):
    for x in range(y):
        print(idx, end=' ')
        idx += 1
    print()