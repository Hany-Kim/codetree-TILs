

a, b = input().split()

if len(a) == len(b):
    print('same')
elif len(a) < len(b):
    print(b, len(b))
else :
    print(a, len(a))