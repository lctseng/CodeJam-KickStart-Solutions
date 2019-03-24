#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace std {
template <> struct hash<pair<int, int>> {
  size_t operator()(const pair<int, int> &p) const {
    return (hash<int>{}(p.first) << 1) ^ (hash<int>{}(p.second));
  }
};
}; // namespace std

namespace Small {

pair<int, int> SHIFTS[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// RxC
int computeSingleDistance(int R, int C, vector<string> &grid, int oi, int oj) {
  // BFS
  queue<pair<int, int>> q;
  q.push(make_pair(oi, oj));
  q.push(make_pair(-1, -1)); // sentinels
  unordered_set<pair<int, int>> seen;
  seen.insert(make_pair(oi, oj));
  int distance = 0;
  while (q.size()) {
    auto current = q.front();
    q.pop();
    if (current.first == -1) {
      // sentinels
      if (q.empty())
        break;
      ++distance;
      q.push(make_pair(-1, -1)); // sentinels
    } else {
      // explore
      for (auto &shift : SHIFTS) {
        int nextI = current.first + shift.first;
        int nextJ = current.second + shift.second;
        if (nextI >= 0 && nextI < R && nextJ >= 0 && nextJ < C) {
          auto nextPair = make_pair(nextI, nextJ);
          if (grid[nextI][nextJ] == '1') {
            // printf("min distance go from %d %d: %d\n", oi, oj, distance + 1);
            return distance + 1;
          } else {
            // need visit this
            if (!seen.count(nextPair)) {
              seen.insert(nextPair);
              q.push(nextPair);
            }
          }
        }
      }
    }
  }
  return distance;
  ;
}

// (RxC)^2
int computeDistance(int R, int C, vector<string> &grid) {
  int maxD = 0;
  // for every point, compute their distance
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      if (grid[i][j] == '0') {
        maxD = max(maxD, computeSingleDistance(R, C, grid, i, j));
      }
    }
  }
  return maxD;
}

// (RxC)^3
int solve(int R, int C, vector<string> &grid) {
  int minD = computeDistance(R, C, grid);
  // pick a new one
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      if (grid[i][j] == '0') {
        grid[i][j] = '1';
        minD = min(minD, computeDistance(R, C, grid));
        grid[i][j] = '0';
      }
    }
  }
  return minD;
}
} // namespace Small

namespace Large {
int solve() {
  // TODO
  return 0;
}
} // namespace Large

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int R, C;
    cin >> R >> C;
    vector<string> grid(R);
    getchar();
    for (int i = 0; i < R; i++) {
      getline(cin, grid[i]);
    }
    cout << "Case #" << t << ": " << Small::solve(R, C, grid) << endl;
  }
  return 0;
}