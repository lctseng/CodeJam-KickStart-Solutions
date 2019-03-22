#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

namespace Simple {

string solveSlidingWindow(int64_t N, int64_t O, int64_t D, vector<int> &S) {
  // simple case: all candy are positive
  // use sliding winodw!
  int64_t curO = 0, curD = 0;
  int i = 0, j = 0;
  // i: leftmost index meet condition
  // j: first index that breaks condition
  bool found = false;
  int64_t maxValue = 0;
  while (true) {
    while (j < N && (S[j] % 2 == 0 || curO + 1 <= O) && (curD + S[j] <= D)) {
      // advance!
      curO += S[j] & 1;
      curD += S[j];
      j++;
    }
    // printf("i = %d, j = %d, curD = %lld, curO = %lld\n", i, j, curD, curO);
    // record if non-empty
    if (j > i && (!found || curD > maxValue)) {
      maxValue = curD;
      found = true;
    }
    // leave loop check
    if (j == N)
      break;
    if (j > i) {
      // advance i by 1
      // update change from i
      curO -= S[i] & 1;
      curD -= S[i];
      ++i;
      // dont need to update changes from J!
      // will be check next loop
    } else {
      // handle empty case
      i = j = j + 1;
    }
  }
  // return string!
  return found ? to_string(maxValue) : "IMPOSSIBLE";
}

string solveEnumerate(int64_t N, int64_t O, int64_t D, vector<int> &S) {
  // enumerate all subset
  vector<int> prefixOdd;
  vector<int64_t> prefixSum;
  prefixOdd.push_back(0);
  prefixSum.push_back(0);
  for (int val : S) {
    prefixSum.push_back(val + prefixSum.back());
    prefixOdd.push_back((val & 1) + prefixOdd.back());
  }
  // enumerate
  int maxValue = 0;
  bool found = false;
  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j <= N; j++) {
      if (prefixOdd[j] - prefixOdd[i] <= O) {
        int64_t curValue = prefixSum[j] - prefixSum[i];
        if (curValue <= D && (!found || curValue > maxValue)) {
          maxValue = curValue;
          found = true;
        }
      }
    }
  }
  // return string!
  return found ? to_string(maxValue) : "IMPOSSIBLE";
}
} // namespace Simple

vector<int> GenerateS(int64_t N, int64_t X1, int64_t X2, int64_t A, int64_t B,
                      int64_t C, int64_t M, int64_t L) {
  vector<int> res;
  res.push_back(X1 + L);
  res.push_back(X2 + L);
  for (int i = 3; i <= N; i++) {
    int64_t X = (A * X2 + B * X1 + C) % M;
    res.push_back(X + L);
    X1 = X2;
    X2 = X;
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int64_t N, O, D;
    cin >> N >> O >> D;
    int64_t X1, X2, A, B, C, M, L;
    cin >> X1 >> X2 >> A >> B >> C >> M >> L;
    auto S = GenerateS(N, X1, X2, A, B, C, M, L);

    cout << "Case #" << t << ": " << Simple::solveSlidingWindow(N, O, D, S)
         << endl;
  }
  return 0;
}