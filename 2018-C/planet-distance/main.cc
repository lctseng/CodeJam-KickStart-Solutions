#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

// algo: use DFS with path recording to find the cycle
// use BFS, from the cycle path as distance 0
// update those distance is INT_MAX

int dfs(vector<vector<int>> &edges, deque<int> &path, vector<bool> &visited,
        int current, int parent) {
  visited[current] = true;
  path.push_back(current);
  // explore
  for (int next : edges[current]) {
    if (next == parent)
      continue;
    if (!visited[next]) {
      // go to this
      int C = dfs(edges, path, visited, next, current);
      if (C != -1) {
        // found cycle
        return C;
      }
    } else {
      // go to the already visited node: cycle!
      return next;
    }
  }
  // go back
  visited[current] = false;
  path.pop_back();
  return -1; // not found in this node
}

deque<int> findCyclePath(int N, vector<vector<int>> &edges) {
  deque<int> path;
  vector<bool> visited(N + 1, false);
  // how to find: if found cycle, return cycle node C immediately, keep path
  // unchanged find first occurance of C
  int C = dfs(edges, path, visited, 1, -1); // pick any start point
  // the rest of it is the cycle node
  // trim the path inplace to save time and space
  while (path.front() != C)
    path.pop_front();
  return path;
}

vector<int> solve(int N, vector<vector<int>> &edges) {
  deque<int> cyclePath = findCyclePath(N, edges);
  vector<int> distance(N + 1, -1);
  queue<int> q;
  for (int c : cyclePath) {
    distance[c] = 0;
    q.push(c);
  }
  q.push(-1); // sentinels
  int currentD = 0;
  // start bfs
  while (true) {
    int current = q.front();
    q.pop();
    if (current > 0) {
      // explore this
      for (int next : edges[current]) {
        if (distance[next] < 0) {
          // un-visited
          distance[next] = currentD + 1;
          q.push(next);
        }
      }
    } else {
      // meet sentinels
      currentD++;
      if (q.empty())
        break;
      q.push(-1); // new sentinels
    }
  }
  return distance;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N;
    cin >> N;
    vector<vector<int>> edges(N + 1);
    for (int i = 0; i < N; i++) {
      int a, b;
      cin >> a >> b;
      edges[a].push_back(b);
      edges[b].push_back(a);
    }
    cout << "Case #" << t << ": ";
    auto res = solve(N, edges);
    for (int i = 1; i <= N - 1; i++) {
      cout << res[i] << ' ';
    }
    cout << res[N] << endl;
  }
  return 0;
}