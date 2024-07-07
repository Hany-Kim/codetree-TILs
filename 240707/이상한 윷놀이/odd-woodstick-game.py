def simulate_game(n, k, board, horses):
    # 방향 정의 (1: 오른쪽, 2: 왼쪽, 3: 위, 4: 아래)
    directions = [(0, 0), (0, 1), (0, -1), (-1, 0), (1, 0)]
    
    # 말의 위치와 방향 초기화
    horse_positions = [(x-1, y-1, d) for x, y, d in horses]
    
    # 격자판에 쌓여있는 말들 초기화
    board_stacks = [[[] for _ in range(n)] for _ in range(n)]
    for i in range(k):
        x, y, _ = horse_positions[i]
        board_stacks[x][y].append(i)
    
    # 말 이동 함수
    def move(horse_idx):
        x, y, d = horse_positions[horse_idx]
        nx, ny = x + directions[d][0], y + directions[d][1]
        
        if nx < 0 or nx >= n or ny < 0 or ny >= n or board[nx][ny] == 2:
            if d == 1: d = 2
            elif d == 2: d = 1
            elif d == 3: d = 4
            elif d == 4: d = 3
            horse_positions[horse_idx] = (x, y, d)
            nx, ny = x + directions[d][0], y + directions[d][1]
            if nx < 0 or nx >= n or ny < 0 or ny >= n or board[nx][ny] == 2:
                return False
        
        stack_idx = board_stacks[x][y].index(horse_idx)
        moving_horses = board_stacks[x][y][stack_idx:]
        board_stacks[x][y] = board_stacks[x][y][:stack_idx]
        
        if board[nx][ny] == 1:
            moving_horses.reverse()
        
        for h in moving_horses:
            horse_positions[h] = (nx, ny, horse_positions[h][2])
            board_stacks[nx][ny].append(h)
        
        if len(board_stacks[nx][ny]) >= 4:
            return True
        
        return False
    
    for turn in range(1, 1001):
        for i in range(k):
            if move(i):
                return turn
    return -1

# 입력 처리
n, k = map(int, input().split())
board = [list(map(int, input().split())) for _ in range(n)]
horses = [tuple(map(int, input().split())) for _ in range(k)]

# 게임 시뮬레이션 실행
result = simulate_game(n, k, board, horses)
print(result)