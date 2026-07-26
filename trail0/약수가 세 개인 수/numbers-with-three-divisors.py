s, e = map(int, input().split())

dp = [0] * 1001

for i in range(1, 1001):
    for j in range(1, 1001):
        if j % i == 0:
            dp[j] += 1

sum = 0
for i in range(s, e+1):
    if dp[i] == 3:
        sum += 1

print(sum)

