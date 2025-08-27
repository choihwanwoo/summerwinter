from collections import deque
import sys

class State:
    def __init__(self):
        self.towers = [[], [], []]  # 각 기둥에 있는 원판들 (맨 아래부터)
        self.moves = []  # 이동 기록
    
    def __str__(self):
        result = ""
        for i in range(3):
            result += f"{i}:"
            for disk in self.towers[i]:
                result += f"{disk},"
            result += ";"
        return result
    
    def __eq__(self, other):
        return self.towers == other.towers
    
    def __hash__(self):
        return hash(str(self))

def solve_hanoi(initial, target):
    """BFS로 최단 경로 찾기"""
    queue = deque([initial])
    visited = {str(initial): True}
    
    while queue:
        current = queue.popleft()
        
        # 목표 상태에 도달했는지 확인
        if current == target:
            return len(current.moves), current.moves
        
        # 가능한 모든 이동 시도
        for from_tower in range(3):
            for to_tower in range(3):
                if from_tower == to_tower:
                    continue
                
                # from 기둥에 원판이 있고, to 기둥이 비어있거나 더 큰 원판 위에 놓을 수 있는 경우
                if (current.towers[from_tower] and 
                    (not current.towers[to_tower] or 
                     current.towers[from_tower][-1] < current.towers[to_tower][-1])):
                    
                    next_state = State()
                    next_state.towers = [tower[:] for tower in current.towers]
                    next_state.moves = current.moves[:]
                    
                    disk = next_state.towers[from_tower].pop()
                    next_state.towers[to_tower].append(disk)
                    next_state.moves.append((from_tower + 1, to_tower + 1))  # 1-based indexing
                    
                    next_str = str(next_state)
                    if next_str not in visited:
                        visited[next_str] = True
                        queue.append(next_state)
    
    return -1, []  # 해결책을 찾을 수 없는 경우

def main():
    # 입력
    n = int(input())
    
    initial = State()
    target = State()
    
    # 초기 상태 입력
    for i in range(3):
        line = list(map(int, input().split()))
        count = line[0]
        for j in range(1, count + 1):
            initial.towers[i].append(line[j])
        # 맨 아래부터 입력받았으므로 뒤집어서 저장
        initial.towers[i].reverse()
    
    # 목표 상태 입력
    for i in range(3):
        line = list(map(int, input().split()))
        count = line[0]
        for j in range(1, count + 1):
            target.towers[i].append(line[j])
        # 맨 아래부터 입력받았으므로 뒤집어서 저장
        target.towers[i].reverse()
    
    # 해결
    move_count, moves = solve_hanoi(initial, target)
    
    # 결과 출력
    print(move_count)
    
    # n ≤ 10인 경우 이동 과정 출력
    if n <= 10:
        for move in moves:
            print(f"{move[0]} {move[1]}")

if __name__ == "__main__":
    main()
