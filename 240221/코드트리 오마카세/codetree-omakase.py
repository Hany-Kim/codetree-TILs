from collections import defaultdict

L, Q = map(int, input().split())  # L: 초밥 벨트의 길이, Q: 명령의 수
sushi_list = [defaultdict(int) for _ in range(L)]  # sushi_list[i]: i번 의자에 놓여있는 초밥, value는 dictionary("sam": 3개)
people_list = defaultdict(list)  # people_list[i]: i이름을 가진 사람, value는 2개(의자 번호, 먹어야될 초밥 개수)
finish_time = 1  # 현재 finish_time까지 한 칸 회전을 완료했음
sushi_cnt = 0
person_cnt = 0

def eat_sushi(name, loc):  # name이름을 가진 사람이 의자 번호 loc 앞에 있을 때 loc에 있는 초밥을 먹음
    global sushi_cnt, person_cnt

    need_to_eat = people_list[name][1]
    current_sushi = sushi_list[loc][name]
    if need_to_eat > current_sushi:  # 먹어야할 스시가 현재 스시의 양보다 많다면
        sushi_list[loc].pop(name)  # 해당 이름의 스시 pop
        people_list[name][1] -= current_sushi  # 해당 개수만큼 스시 먹었다고 표시
    else:  # 먹어야할 스시보다 현재 스시의 양이 많거나 같다면
        people_list.pop(name)  # 해당 이름을 가진 사람 pop
        person_cnt -= 1
        sushi_list[loc][name] -= need_to_eat  # 먹은 스시 개수 감소
    sushi_cnt -= current_sushi  # 총 스시 개수 감소

def move_clockwise(time):  # 현재 시간 time에 회전 요청
    global finish_time, sushi_list

    while time != finish_time:  # 같아질 때까지 한 칸씩 회전
        tmp_sushi_list = [defaultdict(int) for _ in range(L)]
        # 모든 person의 loc이 (-1+L)%L이 됨
        for i in range(L):
            if sushi_list[i] == {}: continue  # 만약 비어있는 의자면 패스
            # i번째 의자에 있는 모든 스시를 i+1번째로 옮기기
            for sushi_name in list(sushi_list[i].keys()):
                next = (i + 1) % L
                tmp_sushi_list[next][sushi_name] += sushi_list[i][sushi_name]
                sushi_list[i].pop(sushi_name)  # 원래 의자에 있던 스시는 pop하기

        sushi_list = [defaultdict(int) for _ in range(L)]

        finish_time += 1
        # 한칸 돌리고 복사
        for i in range(L):
            if tmp_sushi_list[i] == {}: continue
            for sushi_name in tmp_sushi_list[i]:
                sushi_list[i][sushi_name] = tmp_sushi_list[i][sushi_name]

    # 회전이 끝나고 내 앞에 초밥이 놓여 있을 경우
    for person in list(people_list.keys()):
        if sushi_list[people_list[person][0]][person] >= 1:  # 테이블에 자기 이름의 초밥이 놓여져 있다면
            eat_sushi(person, people_list[person][0])

def make_sushi(time, loc, name):
    global sushi_cnt, sushi_list
    if time != 1:  # 처음이 아니면
        move_clockwise(time)  # 시계방향으로 한 칸 회전
    sushi_list[loc][name] += 1  # 위치 loc 앞에 name이름을 부착한 회전 초밥 올려놓음
    sushi_cnt += 1  # 스시 개수 증가

    for person in list(people_list.keys()):
        if sushi_list[loc][person] >= 1:                # 초밥을 놓았는데 해당 의자에 있는 사람이 자기 이름으로 된 초밥이 있다면
            eat_sushi(person, people_list[person][0])

def come_guest(time, loc, name, count):
    global person_cnt
    move_clockwise(time)
    people_list[name] = [loc, count]
    person_cnt += 1
    if name not in sushi_list[loc]: return

    # 만약 시각 time에 위치 loc에 자신의 이름이 적혀있는 초밥이 놓여 있다면
    if sushi_list[loc][name] >= 1:
        eat_sushi(name, loc)

def print_omakase(time):
    # 1. 먼저 초밥 회전이 일어나고, 손님이 자신의 이름이 적힌 초밥을 먹음
    move_clockwise(time)
    # 2. 시간 time에 남아있는 사람 수와 남아 있는 초밥의 수를 출력
    print(person_cnt, sushi_cnt)

for q in range(Q):
    query = input().split()
    if query[0] == "100":
        make_sushi(int(query[1]), int(query[2]), query[3])
    elif query[0] == "200":
        come_guest(int(query[1]), int(query[2]), query[3], int(query[4]))  # 시각t, 위치x, n개의 초밥
    else:
        print_omakase(int(query[1]))  # 시각t