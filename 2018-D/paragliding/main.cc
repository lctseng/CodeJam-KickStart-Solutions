#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

// algo:

namespace Large {

struct Point {
  int x, y;
};

int solve(int N, int K, vector<int> &P, vector<int> &H, vector<int> &X,
          vector<int> &Y) {
  int count = 0;
  auto cmpPointLess = [](const Point &a, const Point &b) { return a.x < b.x; };
  // use 'height relative to origin'
  vector<Point> towers;
  for (int i = 0; i < N; i++) {
    towers.push_back({P[i], H[i]});
  }
  // sort tower according to x
  sort(towers.begin(), towers.end(), cmpPointLess);
  int leftOffset = towers.front().x;
  int rightOffset = towers.back().x;
  // compute towerHeightLeft
  vector<int> towerHeightLeft(N);
  vector<int> towerHeightRight(N);
  for (int i = 0; i < N; i++) {
    auto &tower = towers[i];
    int prevValue = i > 0 ? towerHeightLeft[i - 1] : 0;
    towerHeightLeft[i] = max(tower.y + (tower.x - leftOffset), prevValue);
  }
  for (int i = N - 1; i >= 0; i--) {
    auto &tower = towers[i];
    int prevValue = i < N - 1 ? towerHeightRight[i + 1] : 0;
    towerHeightRight[i] = max(tower.y + (rightOffset - tower.x), prevValue);
  }
  for (int i = 0; i < K; i++) {
    // search from left
    auto leftStartIt =
        upper_bound(towers.begin(), towers.end(), Point{X[i], 0}, cmpPointLess);
    if (leftStartIt != towers.begin()) {
      int towerIndex = leftStartIt - towers.begin() - 1;
      if (towerHeightLeft[towerIndex] >= (Y[i] + X[i] - leftOffset)) {
        // this ballon count!
        count++;
        continue; // go to next ballon
      }
    }
    // search from right
    auto rightStartIt =
        lower_bound(towers.begin(), towers.end(), Point{X[i], 0}, cmpPointLess);
    if (rightStartIt != towers.end()) {
      int towerIndex = rightStartIt - towers.begin();
      if (towerHeightRight[towerIndex] >= (Y[i] + rightOffset - X[i])) {
        // this ballon count!
        count++;
        continue; // go to next ballon
      }
    }
  }
  return count;
}

} // namespace Large

namespace Small {
int solve(int N, int K, vector<int> &P, vector<int> &H, vector<int> &X,
          vector<int> &Y) {
  // simple sol: O(NK): for every ballon, check every pillar
  int count = 0;
  for (int k = 0; k < K; k++) {
    int x = X[k], y = Y[k];
    for (int n = 0; n < N; n++) {
      // can cover by this pillar?
      // h - x_diff >= y
      if (H[n] - abs(x - P[n]) >= y) {
        ++count;
        // no consider
        break;
      }
    }
  }
  return count;
}
} // namespace Small

vector<int> Generate(int size, int64_t v1, int64_t v2, int64_t A, int64_t B,
                     int64_t C, int64_t M) {
  vector<int> res;
  res.push_back(v1);
  res.push_back(v2);
  for (int i = 3; i <= size; i++) {
    int v = (A * v2 + B * v1 + C) % M + 1;
    res.push_back(v);
    v1 = v2;
    v2 = v;
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N, K;
    cin >> N >> K;
    // generate P,H,X,Y
    int64_t v1, v2, A, B, C, M;
    cin >> v1 >> v2 >> A >> B >> C >> M;
    vector<int> P = Generate(N, v1, v2, A, B, C, M);
    cin >> v1 >> v2 >> A >> B >> C >> M;
    vector<int> H = Generate(N, v1, v2, A, B, C, M);
    cin >> v1 >> v2 >> A >> B >> C >> M;
    vector<int> X = Generate(K, v1, v2, A, B, C, M);
    cin >> v1 >> v2 >> A >> B >> C >> M;
    vector<int> Y = Generate(K, v1, v2, A, B, C, M);

    cout << "Case #" << t << ": " << Large::solve(N, K, P, H, X, Y) << endl;
  }
  return 0;
}