#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

struct Condition {
  int A, B, C;
};

// static const auto _ = []() {
//     std::ios::sync_with_stdio( false );
//     std::cin.tie( nullptr );
//     return nullptr;
// }( );

list<char> genBitString(int64_t totalSize, int64_t n) {
  list<char> res;
  while (n) {
    res.push_front((n & 1) + '0');
    n >>= 1;
  }
  // padding
  while (res.size() < totalSize) {
    res.push_front('0');
  }
  return res;
}

string solveBF(int64_t N, int64_t P, vector<Condition> &conditions) {
  int64_t freeDigits = N - conditions.size();
  // generate bitstring from P-1
  auto bitStr = genBitString(freeDigits, P - 1);
  // combine
  string res(N, '0');
  for (auto &cond : conditions) {
    res[cond.A - 1] = '2' + cond.C;
  }
  // use up bitStr
  for (int i = 1; i <= N; i++) {
    char &c = res[i - 1];
    if (c != '2' && c != '3') {
      c = bitStr.front();
      bitStr.pop_front();
    }
  }
  // change 2 to 1
  for (auto &c : res) {
    if (c == '2')
      c = '0';
    else if (c == '3')
      c = '1';
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int64_t N, K, P;
    cin >> N >> K >> P;
    vector<Condition> conditions;
    for (int i = 0; i < K; i++) {
      Condition c;
      cin >> c.A >> c.B >> c.C;
      conditions.push_back(c);
    }
    cout << "Case #" << t << ": " << solveBF(N, P, conditions) << endl;
  }
  return 0;
}