#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

double luckyDip(vector<double> &values, int K) {
  sort(values.begin(), values.end(), greater<int>());
  // base case: K = 0
  double prev =
      accumulate(values.begin(), values.end(), 0.0) / (double)(values.size());
  // iterate for K > 0
  int sz = values.size();
  // divide values
  for (double &val : values) {
    val /= sz;
  }
  // make Avg
  vector<double> avg;
  avg.push_back(values[0]);
  for (int i = 1; i < sz; i++) {
    avg.push_back(avg.back() + values[i]);
  }
  for (int k = 1; k <= K; k++) {
    // iterate first j items to keep
    // binary search! find first values[j-1] that is < prev
    double prevOverSz = prev / sz;
    auto overIt = lower_bound(values.begin(), values.end(), prevOverSz,
                              greater<double>());
    // this points to last valid value, which will be the max
    int j = overIt - values.begin();
    prev = max(prev, avg[j - 1] + (sz - j) * prevOverSz);
  }
  return prev;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    int N, K;
    cin >> N >> K;
    vector<double> values;
    while (N-- > 0) {
      double val;
      cin >> val;
      values.push_back(val);
    }
    printf("Case #%d: %.6lf\n", t, luckyDip(values, K));
  }
  return 0;
}