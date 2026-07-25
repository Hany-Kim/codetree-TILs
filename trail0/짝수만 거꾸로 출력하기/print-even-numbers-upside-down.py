n = int(input())
nums = list(input().split())
nums.reverse()
for i in nums:
    if int(i) % 2 == 0:
        print(i, end=' ')