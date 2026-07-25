s = int(input())
age = int(input())

if s == 0:
    # 남자
    if age >= 19:
        print("MAN")
    else:
        print("BOY")
else :
    # 여자
    if age >= 19:
        print("WOMAN")
    else:
        print("GIRL")