n = int(input())
l = list(input().split())

for i in range(len(l)):
    l[i] = int(l[i]) ** 2
    print(l[i], end=' ')