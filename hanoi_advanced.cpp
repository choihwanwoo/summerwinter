#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
using namespace std;

// 상태를 나타내는 구조체
struct State {
    vector<vector<int>> towers; // 각 기둥에 있는 원판들 (맨 아래부터)
    
    State() {
        towers.resize(3);
    }
    
    // 상태를 문자열로 변환 (방문 체크용)
    string toString() const {
        string result = "";
        for (int i = 0; i < 3; i++) {
            result += to_string(i) + ":";
            for (int disk : towers[i]) {
                result += to_string(disk) + ",";
            }
            result += ";";
        }
        return result;
    }
    
    // 두 상태가 같은지 비교
    bool operator==(const State& other) const {
        return towers == other.towers;
    }
};

// 휴리스틱 함수: 각 원판이 목표 위치에 있지 않으면 1점씩 추가
int heuristic(const State& current, const State& target) {
    int h = 0;
    for (int i = 0; i < 3; i++) {
        if (current.towers[i] != target.towers[i]) {
            h += current.towers[i].size();
        }
    }
    return h;
}

// IDA* 알고리즘으로 최단 경로 찾기
bool ida_star(const State& current, const State& target, int depth, int max_depth, 
              vector<pair<int, int>>& path, unordered_set<string>& visited) {
    
    if (depth + heuristic(current, target) > max_depth) {
        return false;
    }
    
    if (current == target) {
        return true;
    }
    
    if (depth >= max_depth) {
        return false;
    }
    
    string currentStr = current.toString();
    if (visited.find(currentStr) != visited.end()) {
        return false;
    }
    visited.insert(currentStr);
    
    // 가능한 모든 이동 시도
    for (int from = 0; from < 3; from++) {
        for (int to = 0; to < 3; to++) {
            if (from == to) continue;
            
            // from 기둥에 원판이 있고, to 기둥이 비어있거나 더 큰 원판 위에 놓을 수 있는 경우
            if (!current.towers[from].empty() && 
                (current.towers[to].empty() || current.towers[from].back() < current.towers[to].back())) {
                
                State next = current;
                int disk = next.towers[from].back();
                next.towers[from].pop_back();
                next.towers[to].push_back(disk);
                
                path.push_back({from + 1, to + 1}); // 1-based indexing
                
                if (ida_star(next, target, depth + 1, max_depth, path, visited)) {
                    return true;
                }
                
                path.pop_back();
            }
        }
    }
    
    visited.erase(currentStr);
    return false;
}

pair<int, vector<pair<int, int>>> solveHanoi(const State& initial, const State& target) {
    vector<pair<int, int>> path;
    
    // IDA*로 최단 경로 찾기
    for (int max_depth = 0; max_depth <= 100; max_depth++) { // 최대 100단계까지 시도
        unordered_set<string> visited;
        path.clear();
        
        if (ida_star(initial, target, 0, max_depth, path, visited)) {
            return {path.size(), path};
        }
    }
    
    return {-1, {}}; // 해결책을 찾을 수 없는 경우
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    State initial, target;
    
    // 초기 상태 입력
    for (int i = 0; i < 3; i++) {
        int count;
        cin >> count;
        for (int j = 0; j < count; j++) {
            int disk;
            cin >> disk;
            initial.towers[i].push_back(disk);
        }
        // 맨 아래부터 입력받았으므로 뒤집어서 저장
        reverse(initial.towers[i].begin(), initial.towers[i].end());
    }
    
    // 목표 상태 입력
    for (int i = 0; i < 3; i++) {
        int count;
        cin >> count;
        for (int j = 0; j < count; j++) {
            int disk;
            cin >> disk;
            target.towers[i].push_back(disk);
        }
        // 맨 아래부터 입력받았으므로 뒤집어서 저장
        reverse(target.towers[i].begin(), target.towers[i].end());
    }
    
    // 해결
    auto result = solveHanoi(initial, target);
    
    // 결과 출력
    cout << result.first << "\n";
    
    // n ≤ 10인 경우 이동 과정 출력
    if (n <= 10) {
        for (const auto& move : result.second) {
            cout << move.first << " " << move.second << "\n";
        }
    }
    
    return 0;
}
