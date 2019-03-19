#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

int64_t Pow(int64_t a, int64_t b) {
  int64_t res = 1;
  while (b) {
    if (b & 1) {
      --b;
      res *= a;
    } else {
      b >>= 1;
      a *= a;
    }
  }
  return res;
}

bool containsNine(int64_t n) {
  while (n > 0) {
    if (n % 10 == 9)
      return true;
    n /= 10;
  }
  return false;
}

vector<int> decomposition(int64_t n) {
  vector<int> res;
  while (n) {
    res.push_back(n % 10);
    n /= 10;
  }
  if (res.empty())
    res.push_back(0);
  return res;
}

int64_t countDirectlyFrom(int64_t F, int64_t L) {
  int64_t res = 0;
  for (int64_t current = F; current <= L; current++) {
    if (current % 9 && !containsNine(current)) {
      res++;
    }
  }
  return res;
}

int64_t countLessEqualToX(int64_t X) {
  // decomposition X to x[0] to x[n] (LSD to MSD)
  auto Xs = decomposition(X);
  // count directly from [X-x[0], X] => A
  int64_t A = countDirectlyFrom(X - Xs[0], X);
  // let count from 0 to X-x[0]
  // C: number of numbers do not contain 9
  // find number C: number of numbers of base-9 from 0 to X-x[0]
  // by: sum( i from 1 to n ) x[i] * 9**i
  int64_t C = 0;
  for (int i = 1; i < Xs.size(); i++) {
    C += Xs[i] * Pow(9, i);
  }
  // now, remove number from C that is divisible by 9
  // those number can be divide to 9 groups, with tail 0 to 8
  // every 1 group only have 1 divisible by 9, so 8 legal number per group
  // total is B = 8 * (C/9) in [0, X-x[0]]
  int64_t B = 8 * C / 9;
  // final answer is A + B
  return A + B;
}

int64_t solve(int64_t F, int64_t L) {
  // F and  L is gauranteed to be legal
  // plus 1 to re-count F
  return countLessEqualToX(L) - countLessEqualToX(F) + 1;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int64_t F, L;
    cin >> F >> L;
    cout << "Case #" << t << ": " << solve(F, L) << endl;
  }
  return 0;
}