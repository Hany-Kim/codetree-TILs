#include <iostream>

using namespace std;

#define FOR(i,s,e) for(int i=s; i<e; ++i)
#define N_MAX (100000 + 1)
#define Q_MAX (100000);

int N, Q;
struct CHAT {
    int chat_num; // 채팅방 번호
    int auth; // 권한
    bool notice = true; // true = ON | false = OFF
    CHAT* parent = nullptr; // 부모 채팅방
    CHAT* child[2] = { nullptr, nullptr }; // 자식 채팅방
};
CHAT chatting_room_pool[N_MAX];

int dfs(int depth, CHAT* now) {
    int cnt = 1;
    FOR(i, 0, 2) {
        if (now->child[i] == nullptr) continue; // 자식이 없으면 패스
        if (now->child[i]->notice == false) continue; // 스위치 OFF면 패스
        if (now->child[i]->auth >= (depth + 1)) {
            cnt += dfs(depth + 1, now->child[i]);
        }
    }

    return cnt;
}

int notice_check(int num) {
    CHAT* now_chatting_room = &chatting_room_pool[num];

    int cnt = dfs(0, now_chatting_room);
    return cnt;
}

void change_parent(int num1, int num2) {
    CHAT* now1 = &chatting_room_pool[num1];
    CHAT* now2 = &chatting_room_pool[num2];

    CHAT* parent1 = now1->parent;
    CHAT* parent2 = now2->parent;

    int idx1 = -1;
    FOR(i, 0, 2) {
        if (parent1->child[i] == nullptr) continue;
        if (parent1->child[i]->chat_num == num1) {
            idx1 = i;
        }
    }
    int idx2 = -1;
    FOR(i, 0, 2) {
        if (parent2->child[i] == nullptr) continue;
        if (parent2->child[i]->chat_num == num2) {
            idx2 = i;
        }
    }


    parent1->child[idx1] = now2;
    parent2->child[idx2] = now1;

    now1->parent = parent2;
    now2->parent = parent1;
}

void sol() {
    cin >> N >> Q;
    FOR(i, 0, Q) {
        int cmd = 0;
        cin >> cmd;

        if (cmd == 100) {
            FOR(num, 1, (N + 1)) {
                int parent_num = 0;
                cin >> parent_num;

                CHAT* now_chatting_room = &chatting_room_pool[num];
                now_chatting_room->chat_num = num; // 지금 채팅방 번호

                now_chatting_room->parent = &chatting_room_pool[parent_num]; // 자식 -> 부모 연결
                now_chatting_room->parent->chat_num = parent_num; // 부모 채팅방 번호

                FOR(i, 0, 2) {
                    if (now_chatting_room->parent->child[i] == nullptr) {
                        now_chatting_room->parent->child[i] = now_chatting_room; // 부모 -> 자식 연결
                        break;
                    }
                }
            }

            FOR(num, 1, (N + 1)) {
                int auth = 0;
                cin >> auth;

                CHAT* now_chatting_room = &chatting_room_pool[num];
                now_chatting_room->auth = auth;
            }
        }
        else if (cmd == 200) {
            int num = 0;
            cin >> num;

            CHAT* now_chatting_room = &chatting_room_pool[num];
            if (now_chatting_room->notice == true) now_chatting_room->notice = false;
            else if (now_chatting_room->notice == false) now_chatting_room->notice = true;

        }
        else if (cmd == 300) {
            int num = 0;
            int power = 0;
            cin >> num >> power;

            CHAT* now_chatting_room = &chatting_room_pool[num];
            now_chatting_room->auth = power;

        }
        else if (cmd == 400) {
            int num1 = 0;
            int num2 = 0;
            cin >> num1 >> num2;

            change_parent(num1, num2);
        }
        else if(cmd == 500){
            int num = 0;
            cin >> num;

            int cnt = notice_check(num);
            cnt -= 1; // 자기자신 빼기
            cout << cnt << '\n';
        }
    }
}

int main() {
    // 여기에 코드를 작성해주세요.
    sol();
    return 0;
}