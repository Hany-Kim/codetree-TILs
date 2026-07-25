a, b = map(int, input().split())

print(a, end=' ')
print(b, end=' ')

for i in range(0, 8):
    t = a + b
    t %= 10
    print(t, end=' ')
    a = b
    b = t

    