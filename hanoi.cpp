#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map>
using namespace std;

// 간단한 상태 구조체
struct State {
    int towers[3][20];  // 각 기둥에 있는 원판들
    int counts[3];      // 각 기둥의 원판 개수
    vector<pair<int,int>> moves;  // 이동 기록
    
    // 상태를 문자열로 변환 (방문 체크용)
    string toString() {
        string result = "";
        for (int i = 0; i < 3; i++) {
            result += to_string(i) + ":";
            for (int j = 0; j < counts[i]; j++) {
                result += to_string(towers[i][j]) + ",";
            }
            result += ";";
        }
        return result;
    }
    
    // 두 상태가 같은지 비교
    bool equals(State& other) {
        for (int i = 0; i < 3; i++) {
            if (counts[i] != other.counts[i]) return false;
            for (int j = 0; j < counts[i]; j++) {
                if (towers[i][j] != other.towers[i][j]) return false;
            }
        }
        return true;
    }
};

// BFS로 최단 경로 찾기 (학생들이 배우는 기본 알고리즘)
pair<int, vector<pair<int,int>>> solveHanoi(State& initial, State& target) {
    queue<State> q;
    map<string, bool> visited;
    
    q.push(initial);
    visited[initial.toString()] = true;
    
    while (!q.empty()) {
        State current = q.front();
        q.pop();
        
        // 목표 상태에 도달했는지 확인
        if (current.equals(target)) {
            return {current.moves.size(), current.moves};
        }
        
        // 가능한 모든 이동 시도
        for (int from = 0; from < 3; from++) {
            for (int to = 0; to < 3; to++) {
                if (from == to) continue;
                
                // from 기둥에 원판이 있고, to 기둥이 비어있거나 더 큰 원판 위에 놓을 수 있는 경우
                if (current.counts[from] > 0 && 
                    (current.counts[to] == 0 || 
                     current.towers[from][current.counts[from]-1] < current.towers[to][current.counts[to]-1])) {
                    
                    State next = current;
                    
                    // 원판 이동
                    int disk = next.towers[from][next.counts[from]-1];
                    next.counts[from]--;
                    next.towers[to][next.counts[to]] = disk;
                    next.counts[to]++;
                    
                    // 이동 기록 추가
                    next.moves.push_back({from + 1, to + 1});
                    
                    string nextStr = next.toString();
                    if (visited.find(nextStr) == visited.end()) {
                        visited[nextStr] = true;
                        q.push(next);
                    }
                }
            }
        }
    }
    
    return {-1, {}}; // 해결책을 찾을 수 없는 경우
}

int main() {
    int n;
    cin >> n;
    
    State initial, target;
    
    // 초기화
    for (int i = 0; i < 3; i++) {
        initial.counts[i] = 0;
        target.counts[i] = 0;
    }
    
    // 초기 상태 입력
    for (int i = 0; i < 3; i++) {
        int count;
        cin >> count;
        for (int j = 0; j < count; j++) {
            int disk;
            cin >> disk;
            initial.towers[i][j] = disk;
        }
        initial.counts[i] = count;
        // 맨 아래부터 입력받았으므로 뒤집어서 저장
        for (int j = 0; j < count/2; j++) {
            int temp = initial.towers[i][j];
            initial.towers[i][j] = initial.towers[i][count-1-j];
            initial.towers[i][count-1-j] = temp;
        }
    }
    
    // 목표 상태 입력
    for (int i = 0; i < 3; i++) {
        int count;
        cin >> count;
        for (int j = 0; j < count; j++) {
            int disk;
            cin >> disk;
            target.towers[i][j] = disk;
        }
        target.counts[i] = count;
        // 맨 아래부터 입력받았으므로 뒤집어서 저장
        for (int j = 0; j < count/2; j++) {
            int temp = target.towers[i][j];
            target.towers[i][j] = target.towers[i][count-1-j];
            target.towers[i][count-1-j] = temp;
        }
    }
    
    // 해결
    auto result = solveHanoi(initial, target);
    
    // 결과 출력
    cout << result.first << "\n";
    
    // n ≤ 10인 경우 이동 과정 출력
    if (n <= 10) {
        for (int i = 0; i < result.second.size(); i++) {
            cout << result.second[i].first << " " << result.second[i].second << "\n";
        }
    }
    
    return 0;
}
