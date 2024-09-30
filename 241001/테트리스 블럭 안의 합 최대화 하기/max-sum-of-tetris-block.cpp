#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

#define N_MAX 200
#define M_MAX 200
#define BLOCK_CNT 5

struct BLOCK {
   int y, x;
};
vector<vector<BLOCK>> v;
unordered_set<string> blockSet;
int n, m;
int mmap[N_MAX][M_MAX];
int ans;
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, -1, 0, 1 };

void saveBlock(vector<BLOCK> block) {
   string dimentionToStr = "0000000000000000";

   for (int i = 0; i < block.size(); ++i) {
      BLOCK now = block[i];

      int idx = (4 * now.y) + now.x;
      dimentionToStr[idx] = '1';
   }

   blockSet.insert(dimentionToStr);
}

void rotateAndSaveBlock(vector<BLOCK> block) {
   vector<BLOCK> last = block;
   for (int rNum = 0; rNum < 3; ++rNum) {
      vector<BLOCK> cur;
      BLOCK zeroPoint = { INT_MAX, INT_MAX };

      for (int i = 0; i < last.size(); ++i) {
         BLOCK rotatePos;
         rotatePos.y = 4 - 1 - last[i].x;
         rotatePos.x = last[i].y;

         zeroPoint.y = min(zeroPoint.y, rotatePos.y);
         zeroPoint.x = min(zeroPoint.x, rotatePos.x);
         cur.push_back(rotatePos);
      }

      for (int i = 0; i < cur.size(); ++i) {
         cur[i].y = cur[i].y - zeroPoint.y;
         cur[i].x = cur[i].x - zeroPoint.x;
      }

      saveBlock(cur);
      last = cur;
   }
}

void reverseAndRotateAndSaveBlock(vector<BLOCK> block) {
    vector<BLOCK> last = block;

    vector<BLOCK> cur;
    BLOCK zeroPoint = { INT_MAX, INT_MAX };

    for (int i = 0; i < last.size(); ++i) {
        BLOCK reversePos;
        reversePos.y = last[i].y;
        reversePos.x = 4 - 1 - last[i].x;

        zeroPoint.y = min(zeroPoint.y, reversePos.y);
        zeroPoint.x = min(zeroPoint.x, reversePos.x);
        cur.push_back(reversePos);
    }

    for (int i = 0; i < cur.size(); ++i) {
        cur[i].y = cur[i].y - zeroPoint.y;
        cur[i].x = cur[i].x - zeroPoint.x;
    }

    saveBlock(cur);
    
    rotateAndSaveBlock(cur);
}

void makeBlock() {
   for (int i = 0; i < BLOCK_CNT; ++i) {
      saveBlock(v[i]);
      rotateAndSaveBlock(v[i]);
      reverseAndRotateAndSaveBlock(v[i]);
   }
}

void makeShapes() {
   v = vector<vector<BLOCK>>(BLOCK_CNT, vector<BLOCK>(4));
   v[0][0] = { 0,0 };
   v[0][1] = { 0,1 };
   v[0][2] = { 0,2 };
   v[0][3] = { 0,3 };

   v[1][0] = { 0,0 };
   v[1][1] = { 0,1 };
   v[1][2] = { 1,0 };
   v[1][3] = { 1,1 };

   v[2][0] = { 0,0 };
   v[2][1] = { 1,0 };
   v[2][2] = { 2,0 };
   v[2][3] = { 2,1 };

   v[3][0] = { 0,0 };
   v[3][1] = { 1,0 };
   v[3][2] = { 1,1 };
   v[3][3] = { 2,1 };

   v[4][0] = { 0,0 };
   v[4][1] = { 1,0 };
   v[4][2] = { 1,1 };
   v[4][3] = { 2,0 };
}

BLOCK getStartPos(string strBlock) {
    BLOCK ret = { 0, 0 };

    int idx = strBlock.find('1', 0);
    ret.y = idx / 4;
    ret.x = idx % 4;

    return ret;
}

void makeBlockArr(int tmp[4][4], string str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '0') continue;
        int y = i / 4;
        int x = i % 4;
        tmp[y][x] = 1;
    }
}

void bfs(BLOCK start, BLOCK blockPos, int block[4][4]) {
    queue<pair<BLOCK, BLOCK>> q;
    int used[N_MAX][M_MAX] = { 0, };
    int cnt = 0;
    int sum = 0;

    BLOCK s;
    s.y = start.y + blockPos.y;
    s.x = start.x + blockPos.x;

    if (s.y < 0 || s.y >= n || s.x < 0 || s.x >= m) return;
    q.push(make_pair(s, blockPos));
    used[s.y][s.x] = 1;

    while (!q.empty()) {
        pair<BLOCK, BLOCK> now = q.front(); q.pop();
        ++cnt;
        sum += mmap[now.first.y][now.first.x];

        for (int i = 0; i < 4; ++i) {
            int by = now.second.y + dy[i];
            int bx = now.second.x + dx[i];

            if (by < 0 || by >= 4 || bx < 0 || bx >= 4)continue;
            if (block[by][bx] == 0) continue;

            int ny = now.first.y + dy[i];
            int nx = now.first.x + dx[i];

            if (ny < 0 || ny >= n || nx < 0 || nx >= m) continue;
            if (used[ny][nx] == 1) continue;

            used[ny][nx] = 1;
            pair<BLOCK, BLOCK> next;
            next.first.y = ny;
            next.first.x = nx;
            next.second.y = by;
            next.second.x = bx;

            q.push(next);
        }
    }

    if (cnt == 4) {
        ans = max(ans, sum);
    }
}

void calc() {
    for (unordered_set<string>::iterator itr = blockSet.begin(); itr != blockSet.end(); itr++) {
        BLOCK findStartPos = getStartPos(*itr);
        
        int tmp[4][4] = { 0, };
        makeBlockArr(tmp, *itr);

        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                BLOCK start;
                start.y = y;
                start.x = x;
                bfs(start, findStartPos, tmp);
            }
        }
    }
}

void sol() {
   makeShapes();
   makeBlock();
   //calc();
   cout << ans;
}

void in() {
    cin >> n >> m;
    for (int y = 0; y < n; ++y) {
        for (int x = 0; x < m; ++x) {
            cin >> mmap[y][x];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    //freopen("input.txt", "r", stdin);
    in();
    sol();
    return 0;
}