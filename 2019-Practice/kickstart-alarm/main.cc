#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define MOD 1000000007

int64_t PowMod(int64_t a, int64_t b) {
  int64_t res = 1;
  while (b) {
    if (b & 1) {
      --b;
      res = (res * a) % MOD;
    } else {
      b >>= 1;
      a = (a * a) % MOD;
    }
  }
  return res;
}

vector<int> GenA(int N, int64_t x1, int64_t y1, int64_t C, int64_t D,
                 int64_t E1, int64_t E2, int64_t F) {
  vector<int> res;
  int64_t x = x1;
  int64_t y = y1;
  res.push_back((x1 + y1) % F);
  for (int i = 2; i <= N; i++) {
    int nextX = (C * x + D * y + E1) % F;
    int nextY = (D * x + C * y + E2) % F;
    res.push_back((nextX + nextY) % F);
    x = nextX, y = nextY;
  }
  return move(res);
}

int solveBF(int N, int K, int x1, int y1, int C, int D, int E1, int E2, int F) {
  auto A = GenA(N, x1, y1, C, D, E1, E2, F);
  int sum = 0;
  for (int i = 1; i <= K; i++) {
    // enumerate all subarray
    for (int first = 0; first < N; first++) {
      for (int last = first; last < N; last++) {
        // subarray [first:last]
        for (int index = first; index <= last; ++index) {
          int base = index - first + 1;
          int64_t basePow = PowMod(base, i);
          sum = (sum + basePow * A[index]) % MOD;
        }
      }
    }
  }
  return sum;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    // TODO: read input for each case and compute output
    int N, K, x1, y1, C, D, E1, E2, F;
    cin >> N >> K >> x1 >> y1 >> C >> D >> E1 >> E2 >> F;
    cout << "Case #" << t << ": " << solveBF(N, K, x1, y1, C, D, E1, E2, F)
         << endl;
  }
  return 0;
}