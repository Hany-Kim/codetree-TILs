#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

#define FOR(i,s,e) for(int i=s; i<e; ++i)
#define Q_MAX 10

int L, Q;
struct CMD {
    int cmd, x, t, n;
    string name;
};
vector<CMD> commands;
//vector<CMD> create_sushi;
unordered_map<string, vector<CMD>> create_sushi;
unordered_map<string, int> sitting_position; // 사람이 앉은 자리 매핑
unordered_set<string> visitting_customer_names; // 방문한 사람 저장 ( 재방문은 없으므로 중복이 없다 )
unordered_map<string, int> visit_customer_time;
unordered_map<string, int> leave_customer_time;
unordered_map<string, int> eat_cnt;
vector<CMD> v;
int sushi_cnt;
int customer_cnt;

void input() {
    cin >> L >> Q;
    FOR(i, 0, Q) {
        int cmd = -1, t = -1, x = -1, n = -1;
        string name = "";

        cin >> cmd;

        if (cmd == 100) {
            cin >> t >> x >> name;
            CMD tmp1;
            tmp1.cmd = cmd;
            tmp1.t = t;
            tmp1.x = x;
            tmp1.name = name;
            tmp1.n = n;
            //create_sushi.push_back(tmp1);
            create_sushi[name].push_back(tmp1);
        }
        else if (cmd == 200) {
            cin >> t >> x >> name >> n;

            visitting_customer_names.insert(name);
            sitting_position[name] = x; // 오는 손님은 모두 다르다 ( 재방문 x )
            visit_customer_time[name] = t; // 손님이 방문한 시각
            eat_cnt[name] = n;
        }
        else if (cmd == 300) cin >> t;

        CMD tmp;
        tmp.cmd = cmd;
        tmp.t = t;
        tmp.x = x;
        tmp.name = name;
        tmp.n = n;
        commands.push_back(tmp);
    }
}

bool mstrcmp(string str1, string str2) {
    if (str1.size() != str2.size()) return false;
    else {
        FOR(i, 0, str1.size()) {
            if (str1[i] != str2[i]) return false;
        }
        return true;
    }
}

bool mcmp(CMD left, CMD right) {
    if (left.t > right.t) return false;
    if (left.t < right.t) return true;

    if (left.cmd > right.cmd) return false;
    if (left.cmd < right.cmd) return true;

    return false;
}

void sol() {
    for (string now_name : visitting_customer_names) {
        leave_customer_time[now_name] = -1;

        //FOR(i, 0, create_sushi.size()) { // 생성된 초밥이 언제 없어질지 기록
        for(CMD sushis : create_sushi[now_name]) {// 생성된 초밥이 언제 없어질지 기록
            //if (mstrcmp(create_sushi[i].name, now_name) == false) continue;

            int meet_sushi_cutomer_time = 0; // 초밥과 손님이 만나게 할 수 있는 보정 시간
            int eat_sushi_time = 0; //초밥을 먹는 시간
            if (sushis.t < visit_customer_time[now_name]) { // 손님 입장전에 손님의 초밥이 벨트위에 있음
                int now_sushi_position = sushis.x;

                int diff_t = visit_customer_time[now_name] - sushis.t;
                now_sushi_position = ((now_sushi_position + diff_t) % L); // 손님이 입장했을때, 회전 초밥 위치 이동

                if (sitting_position[now_name] >= now_sushi_position) {
                    meet_sushi_cutomer_time = sitting_position[now_name] - now_sushi_position;
                }
                else if (sitting_position[now_name] < now_sushi_position) {
                    meet_sushi_cutomer_time = (sitting_position[now_name] + L) - now_sushi_position;
                }
                eat_sushi_time = visit_customer_time[now_name] + meet_sushi_cutomer_time;
            }
            else { // 손님 입장 후 초밥이 벨트위로 올라옴
                int now_sushi_position = sushis.x;

                if (sitting_position[now_name] >= now_sushi_position) {
                    meet_sushi_cutomer_time = sitting_position[now_name] - now_sushi_position;
                }
                else if (sitting_position[now_name] < now_sushi_position) {
                    meet_sushi_cutomer_time = (sitting_position[now_name] + L) - now_sushi_position;
                }
                eat_sushi_time = sushis.t + meet_sushi_cutomer_time;
            }

            CMD tmp;
            tmp.cmd = sushis.cmd + 1;
            tmp.t = eat_sushi_time;
            tmp.x = -1;
            tmp.name = now_name;
            tmp.n = -1;
            v.push_back(tmp);
        }
    }

    commands.insert(commands.end(), v.begin(), v.end());
    sort(commands.begin(), commands.end(), mcmp);

    FOR(i, 0, commands.size()) {
        if (commands[i].cmd == 100) {
            ++sushi_cnt;
        }
        else if (commands[i].cmd == 101) {
            --sushi_cnt;

            string now_name = commands[i].name;
            eat_cnt[now_name] = eat_cnt[now_name] - 1;
            if (eat_cnt[now_name] <= 0) --customer_cnt;
        }
        else if (commands[i].cmd == 200) {
            ++customer_cnt;
        }
        else if (commands[i].cmd == 300) {
            cout << customer_cnt << " " << sushi_cnt << '\n';
        }
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    //freopen("input.txt", "r", stdin);
    input();
    sol();

    return 0;
}