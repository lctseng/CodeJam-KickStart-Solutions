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

namespace Small {
int solve(int N, int P, vector<int> &students) {
  sort(students.begin(), students.end());
  // collect P
  int sum = 0;
  for (int i = 0; i < P - 1; i++) {
    sum += students[P - 1] - students[i];
  }
  int minSum = sum;
  // consider  from P-th
  for (int i = P; i < N; i++) {
    // reject
    sum -= students[i - 1] - students[i - P];
    sum += (P - 1) * (students[i] - students[i - 1]);
    if (sum < minSum)
      minSum = sum;
  }
  // sub
  return minSum;
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
    int N, P;
    cin >> N >> P;
    vector<int> students;
    for (int i = 0; i < N; i++) {
      int s;
      cin >> s;
      students.push_back(s);
    }
    cout << "Case #" << t << ": " << Small::solve(N, P, students) << endl;
  }
  return 0;
}