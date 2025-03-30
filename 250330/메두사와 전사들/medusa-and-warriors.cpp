#include <iostream>
#include <queue>
#include <cstring>
#include <stdlib.h>
#include <climits>
#include <math.h>
using namespace std;

#define N_MAX (50)
#define M_MAX (300)

int N, M;
struct POS {
	int y, x;
};
struct JEONSA {
	int y, x;
	bool isDie, isStone; // 살아 있는지, 돌인지
};
int map[N_MAX][N_MAX]; // 도로 0, 벽 1
int jeonsa_map[N_MAX][N_MAX];
POS house, park, medusa;
int dist[N_MAX][N_MAX]; // 공원 ~> 집 최단 거리
int siya[N_MAX][N_MAX]; // 시야맵
JEONSA jeonsa[M_MAX];
int dy[4][3] = {
	{-1, -1, -1},
	{1, 1, 1},
	{-1, 0, 1},
	{-1, 0, 1}
};
int dx[4][3] = {
	{-1, 0, 1},
	{-1, 0, 1},
	{-1, -1, -1},
	{1, 1, 1}
};

void bfs() {
	// 공원 ~> 집 최단 거리
	queue<POS> q;

	POS start = park;
	dist[start.y][start.x] = 1;
	q.push(start);

	while (!q.empty()) {
		POS now = q.front(); q.pop();

		for (int i = 0; i < 4; ++i) {
			int ny = now.y + dy[i][1];
			int nx = now.x + dx[i][1];

			if (ny < 0 || ny >= N || nx < 0 || nx >= N)continue; // 격자 밖 x
			if (map[ny][nx] == 1) continue; //벽 x
			if (dist[ny][nx] != 0) continue; // 방문 처리

			dist[ny][nx] = dist[now.y][now.x] + 1;
			q.push({ ny, nx });
		}
	}
}

void move_medusa() {
	// 4방향 확인 -> 1칸 선택
	int selected_dir = -1; // 선택한 방향
	int min_dist = dist[medusa.y][medusa.x]; // 현재 메두사 위치 ~ 공원 거리
	for (int i = 0; i < 4; ++i) {
		int ny = medusa.y + dy[i][1];
		int nx = medusa.x + dx[i][1];

		if (ny < 0 || ny >= N || nx < 0 || nx >= N)continue; // 격자 밖 x
		if (map[ny][nx] == 1) continue; //벽 x
		if (dist[ny][nx] == 0) continue; // 최단거리에 구할 때 방문하지 않은 곳 pass

		if (min_dist > dist[ny][nx]) {
			min_dist = dist[ny][nx];
			selected_dir = i;
		}
	}

	if (selected_dir == -1) {
		cout << "다음 위치 선택 못한 경우 있음\n";
	}

	POS next;
	next.y = medusa.y + dy[selected_dir][1];
	next.x = medusa.x + dx[selected_dir][1];

	// 이동할 위치에 전사 있으면 죽임
	if (jeonsa_map[next.y][next.x] == 1) {
		// 이동할 위치에 전사 있음.
		int new_jeonsa_map[N_MAX][N_MAX] = { 0, };

		for (int i = 0; i < M; ++i) {
			JEONSA* now = &jeonsa[i];
			if (now->isDie == true) continue;

			if (now->y == next.y && now->x == next.x) {
				jeonsa[i].isDie = true;
			}

			new_jeonsa_map[now->y][now->x] = 1;
		}

		memcpy(jeonsa_map, new_jeonsa_map, sizeof(jeonsa_map));
	}

	// 이동
	map[medusa.y][medusa.x] = 0;
	medusa = next;
	map[medusa.y][medusa.x] = 2;
}

int sight_of_medusa() {
	int stone_cnt = 0;

	// 4방향 시야 확인 중 -> 전사가 돌이 가장 많이 된 곳 1개 선택
	for (int dir = 0; dir < 4; ++dir) {
		queue<POS> q;
		int tmp_siya[N_MAX][N_MAX] = { 0, }; // 임시 시야맵

		POS start = medusa;
		tmp_siya[start.y][start.x] = 1;
		q.push(start);

		while (!q.empty()) {
			POS now = q.front(); q.pop();

			for (int i = 0; i < 3; ++i) {
				int ny = now.y + dy[dir][i];
				int nx = now.x + dx[dir][i];

				if (ny < 0 || ny >= N || nx < 0 || nx >= N)continue; // 격자밖
				if (tmp_siya[ny][nx] != 0) continue;

				if (jeonsa_map[ny][nx] == 1) {
					// 전사와 마주침

					// 전사 ~ 메두사 일직선상
					if (medusa.y == ny || medusa.x == nx) {
						// 현재 전사는 돌이됨.
						tmp_siya[ny][nx] = 1;

						// 현재 전사 뒤로는 가려짐.
						int nny = ny;
						int nnx = nx;

						while (true) {
							nny = nny + dy[dir][1];
							nnx = nnx + dx[dir][1];

							if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) break;
							if (tmp_siya[nny][nnx] != 0) break;

							tmp_siya[nny][nnx] = 2;
						}
					}

					// 전사 ~ 메두사 사선
					else if (ny < medusa.y && nx > medusa.x) {
						// 1사분면

						queue<POS> nq;

						POS ns = { ny, nx };
						tmp_siya[ns.y][ns.x] = 1;
						nq.push(ns);

						while (!nq.empty()) {
							POS nnow = nq.front(); nq.pop();

							if (dir == 0) {
								// 위
								for (int j = 1; j < 3; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
							if (dir == 3) {
								// 오른쪽
								for (int j = 0; j < 2; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
						}
					}
					else if (ny < medusa.y && nx < medusa.x) {
						// 2사분면

						queue<POS> nq;

						POS ns = { ny, nx };
						tmp_siya[ns.y][ns.x] = 1;
						nq.push(ns);

						while (!nq.empty()) {
							POS nnow = nq.front(); nq.pop();

							if (dir == 0) {
								// 위
								for (int j = 0; j < 2; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
							if (dir == 2) {
								// 왼쪽
								for (int j = 0; j < 2; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
						}
					}
					else if (ny > medusa.y && nx < medusa.x) {
						// 3사분면

						queue<POS> nq;

						POS ns = { ny, nx };
						tmp_siya[ns.y][ns.x] = 1;
						nq.push(ns);

						while (!nq.empty()) {
							POS nnow = nq.front(); nq.pop();

							if (dir == 1) {
								// 아래
								for (int j = 0; j < 2; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
							if (dir == 2) {
								// 왼쪽
								for (int j = 1; j < 3; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
						}
					}
					else if (ny > medusa.y && nx > medusa.x) {
						// 4사분면

						queue<POS> nq;

						POS ns = { ny, nx };
						tmp_siya[ns.y][ns.x] = 1;
						nq.push(ns);

						while (!nq.empty()) {
							POS nnow = nq.front(); nq.pop();

							if (dir == 1) {
								// 아래
								for (int j = 1; j < 3; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
							if (dir == 3) {
								// 오른쪽
								for (int j = 1; j < 3; ++j) {
									int nny = nnow.y + dy[dir][j];
									int nnx = nnow.x + dx[dir][j];

									if (nny < 0 || nny >= N || nnx < 0 || nnx >= N) continue; // 격자밖
									if (tmp_siya[nny][nnx] != 0) continue;

									tmp_siya[nny][nnx] = 2;
									nq.push({ nny, nnx });
								}
							}
						}
						}
				}
				else {
					tmp_siya[ny][nx] = 1;
					q.push({ ny, nx });
				}
			}
		}
	
		// 현재 방향에서 돌이된 전사 수 구하기
		int cnt = 0;
		for (int i = 0; i < M; ++i) {
			JEONSA* now = &jeonsa[i];
			if (now->isDie) continue;

			if (tmp_siya[now->y][now->x] == 1) {
				// 돌이 될 위치에 있음.
				cnt += 1;
			}
		}

		if (stone_cnt < cnt) {
			// 시야 방향 선택
			tmp_siya[medusa.y][medusa.x] = 0;
			memcpy(siya, tmp_siya, sizeof(siya));
			stone_cnt = cnt;
		}
	}

	// 어떤 전사는 다른 전사에게 가려질 수 있음 -> 2


	// 시야내 전사는 돌이 됨 ( 같은 위치에 전사 여러명이면, 모두 돌 )
	int new_jeonsa_map[N_MAX][N_MAX] = { 0, };
	for (int i = 0; i < M; ++i) {
		JEONSA* now = &jeonsa[i];
		if (now->isDie) continue;

		if (siya[now->y][now->x] == 1) {
			// 돌이 됨
			now->isStone = true;
		}

		new_jeonsa_map[now->y][now->x] = 1;
	}
	memcpy(jeonsa_map, new_jeonsa_map, sizeof(jeonsa_map));

	return stone_cnt;
}

int get_mh_dist(POS left, POS right) {
	return abs(left.y - right.y) + abs(left.x - right.x);
}

int move_jeonsa() {
	int move_cnt = 0;

	int new_jeonsa_map[N_MAX][N_MAX] = { 0, };
	for (int i = 0; i < M; ++i) {
		JEONSA* now = &jeonsa[i];

		if (now->isDie) continue;
		if (now->isStone) {
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		// 첫번째 이동 (상하 좌우)
		int cur_dist = get_mh_dist({ now->y, now->x }, medusa);

		POS next;
		int min_dist = cur_dist;
		for (int j = 0; j < 4; ++j) {
			int ny = now->y + dy[j][1];
			int nx = now->x + dx[j][1];

			if (ny < 0 || ny >= N || nx < 0 || nx >= N)continue;
			if (siya[ny][nx] == 1) continue;

			int next_dist = get_mh_dist({ ny, nx }, medusa);

			if (next_dist < min_dist) {
				min_dist = next_dist;
				next = { ny, nx };
			}
		}

		if (min_dist == cur_dist) {
			// 이동할 곳이 없음.
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		move_cnt += 1;
		now->y = next.y;
		now->x = next.x;

		if (now->y == medusa.y && now->x == medusa.x) {
			// 메두사와 만남
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		cur_dist = min_dist;
		
		// 두번째 이동 (좌우 상하)
		min_dist = cur_dist;

		for (int j = 0; j < 4; ++j) {
			int nj = j + 2;
			if (nj >= 4) nj -= 4;

			int ny = now->y + dy[nj][1];
			int nx = now->x + dx[nj][1];

			if (ny < 0 || ny >= N || nx < 0 || nx >= N)continue;
			if (siya[ny][nx] == 1) continue;

			int next_dist = get_mh_dist({ ny, nx }, medusa);

			if (next_dist < min_dist) {
				min_dist = next_dist;
				next = { ny, nx };
			}
		}

		if (min_dist == cur_dist) {
			// 이동할 곳이 없음.
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		move_cnt += 1;
		now->y = next.y;
		now->x = next.x;

		if (now->y == medusa.y && now->x == medusa.x) {
			// 메두사와 만남
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		new_jeonsa_map[now->y][now->x] = 1;
	}

	memcpy(jeonsa_map, new_jeonsa_map, sizeof(jeonsa_map));
	return move_cnt;
}

int attack_jeonsa() {
	int attack_cnt = 0;

	int new_jeonsa_map[N_MAX][N_MAX] = { 0, };
	for (int i = 0; i < M; ++i) {
		JEONSA* now = &jeonsa[i];

		if (now->isDie) continue;
		if (now->isStone) {
			new_jeonsa_map[now->y][now->x] = 1;
			continue;
		}

		if (now->y == medusa.y && now->x == medusa.x) {
			// 전사는 메두사 공격 후 죽음
			attack_cnt += 1;
			now->isDie = true;
		}
		else new_jeonsa_map[now->y][now->x] = 1;
	}

	memcpy(jeonsa_map, new_jeonsa_map, sizeof(jeonsa_map));

	return attack_cnt;
}

void sol() {
	bfs();

	if (dist[house.y][house.x] == 0) {
		cout << "-1";
		return;
	}

	while (true) {
		// 1. 메두사 이동
		move_medusa();
		if (medusa.y == park.y && medusa.x == park.x) {
			cout << 0;
			break;
		}

		// 2. 메두사 시선
		int stone_cnt = sight_of_medusa();

		// 3. 전사들의 이동
		int move_cnt = move_jeonsa();

		// 4. 전사 공격
		int attack_cnt = attack_jeonsa();

		cout << move_cnt << ' ' << stone_cnt << ' ' << attack_cnt << '\n';

		// 마지막. 돌해제
		for (int i = 0; i < M; ++i) {
			JEONSA* now = &jeonsa[i];
			if (now->isStone) {
				now->isStone = false;
			}
		}
	}

	int de = 1;
}

void input() {
	cin >> N >> M;
	cin >> house.y >> house.x >> park.y >> park.x;

	for (int i = 0; i < M; ++i) {
		cin >> jeonsa[i].y >> jeonsa[i].x;
		jeonsa[i].isDie = false;
		jeonsa[i].isStone = false;

		// 지도에 전사위치 표시
		jeonsa_map[jeonsa[i].y][jeonsa[i].x] = 1;
	}

	for (int y = 0; y < N; ++y) {
		for (int x = 0; x < N; ++x) {
			cin >> map[y][x];
		}
	}

	medusa = house;
	map[medusa.y][medusa.x] = 2;
}

int main() {

	input();
	sol();

	return 0;
}