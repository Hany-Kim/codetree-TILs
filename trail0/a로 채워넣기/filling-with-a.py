s = input()

s = s[:1] + 'a' + s[2:]
s = s[:-2] + 'a' + s[-1:]

print(s)