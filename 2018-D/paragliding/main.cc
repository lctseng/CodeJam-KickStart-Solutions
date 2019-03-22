#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

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
  // cout << "Generated: ";
  // for (int val : res) {
  //   cout << val << ' ';
  // }
  // cout << endl;
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

    cout << "Case #" << t << ": " << Small::solve(N, K, P, H, X, Y) << endl;
  }
  return 0;
}