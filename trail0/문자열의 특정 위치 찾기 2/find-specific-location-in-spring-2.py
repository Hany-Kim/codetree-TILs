fruits = ["apple", "banana", "grape", "blueberry", "orange"]

c = input()
cnt = 0

for f in fruits:
    if c == f[2] or c == f[3]:
        print(f)
        cnt += 1

print(cnt)